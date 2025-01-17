/*
 * Copyright 2021 NSG650
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "devtmpfs.h"
#include "../klibc/lock.h"
#include "../klibc/mem.h"
#include "../klibc/resource.h"
#include "vfs.h"
#include <liballoc.h>
#include <stddef.h>

struct tmpfs_resource {
	struct resource res;
	size_t allocated_size;
	char *data;
};

static ino_t inode_counter = 1;

static struct vfs_node devfs_mount_gate = {.name = "/dev",
										   .res = NULL,
										   .mount_data = NULL,
										   .fs = &devtmpfs,
										   .mount_gate = NULL,
										   .parent = NULL,
										   .child = NULL,
										   .next = NULL,
										   .backing_dev_id = 0};

bool devtmpfs_add_device(struct resource *res, const char *name) {
	struct vfs_node *new_node = vfs_new_node_deep(&devfs_mount_gate, name);

	if (new_node == NULL)
		return false;

	new_node->res = res;

	return true;
}

static struct vfs_node *devtmpfs_mount(struct resource *device) {
	(void)device;

	return &devfs_mount_gate;
}

static ssize_t devtmpfs_read(struct resource *_this, void *buf, off_t off,
							 size_t count) {
	struct tmpfs_resource *this = (void *)_this;
	LOCK(this->res.lock);

	if (off + count > (size_t)this->res.st.st_size)
		count -= (off + count) - this->res.st.st_size;

	memcpy(buf, this->data + off, count);
	UNLOCK(this->res.lock);
	return count;
}

static ssize_t devtmpfs_write(struct resource *_this, const void *buf,
							  off_t off, size_t count) {
	struct tmpfs_resource *this = (void *)_this;
	LOCK(this->res.lock);

	if (off + count > this->allocated_size) {
		while (off + count > this->allocated_size)
			this->allocated_size *= 2;

		this->data = krealloc(this->data, this->allocated_size);
	}

	memcpy(this->data + off, buf, count);

	this->res.st.st_size += count;
	UNLOCK(this->res.lock);
	return count;
}

static int devtmpfs_close(struct resource *_this) {
	struct tmpfs_resource *this = (void *)_this;
	LOCK(this->res.lock);
	this->res.refcount--;
	UNLOCK(this->res.lock);
	return 0;
}

static struct resource *devtmpfs_open(struct vfs_node *node, bool create,
									  mode_t mode) {
	if (!create)
		return NULL;

	struct tmpfs_resource *res = resource_create(sizeof(struct tmpfs_resource));

	res->allocated_size = 4096;
	res->data = kmalloc(res->allocated_size);
	res->res.st.st_dev = node->backing_dev_id;
	res->res.st.st_size = 0;
	res->res.st.st_blocks = 0;
	res->res.st.st_blksize = 512;
	res->res.st.st_ino = inode_counter++;
	res->res.st.st_mode = (mode & ~S_IFMT) | S_IFREG;
	res->res.st.st_nlink = 1;
	res->res.close = devtmpfs_close;
	res->res.read = devtmpfs_read;
	res->res.write = devtmpfs_write;

	return (void *)res;
}

static struct resource *devtmpfs_mkdir(struct vfs_node *node, mode_t mode) {
	struct resource *res = resource_create(sizeof(struct resource));

	res->st.st_dev = node->backing_dev_id;
	res->st.st_size = 0;
	res->st.st_blocks = 0;
	res->st.st_blksize = 512;
	res->st.st_ino = inode_counter++;
	res->st.st_mode = (mode & ~S_IFMT) | S_IFDIR;
	res->st.st_nlink = 1;

	return (void *)res;
}

static struct vfs_node *devtmpfs_populate(struct vfs_node *node) {
	(void)node;
	return NULL;
}

struct filesystem devtmpfs = {.name = "devtmpfs",
							  .needs_backing_device = false,
							  .mount = devtmpfs_mount,
							  .open = devtmpfs_open,
							  .mkdir = devtmpfs_mkdir,
							  .populate = devtmpfs_populate};
