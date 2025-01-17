; Copyright 2021 NSG650
; Copyright 2021 Sebastian
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
;     http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.

%include "kernel/cpu/stackop.inc"

extern isr_handler

; Common handler for the ISRs
isr_common_format:
	pushall
	cld
	mov rdi, rsp
	call isr_handler
	popall
	add rsp, 24
	iretq

%macro isr 1

global isr%1
isr%1:
	push 0
	push %1
	push fs
	jmp isr_common_format

%endmacro

%macro errorIsr 1

global errorIsr%1
errorIsr%1:
	push %1
	push fs
	jmp isr_common_format

%endmacro

; Define ISRs
isr 0
isr 1
isr 2
isr 3
isr 4
isr 5
isr 6
isr 7
errorIsr 8
isr 9
errorIsr 10
errorIsr 11
errorIsr 12
errorIsr 13
errorIsr 14
isr 15
isr 16
isr 17
isr 18
isr 19
isr 20
isr 21
isr 22
isr 23
isr 24
isr 25
isr 26
isr 27
isr 28
isr 29
isr 30
isr 31
isr 32
isr 33
isr 34
isr 35
isr 36
isr 37
isr 38
isr 39
isr 40
isr 41
isr 42
isr 43
isr 44
isr 45
isr 46
isr 47
isr 48
isr 49
isr 50
isr 51
isr 52
isr 53
isr 54
isr 55
isr 56
isr 57
isr 58
isr 59
isr 60
isr 61
isr 62
isr 63
isr 64
isr 65
isr 66
isr 67
isr 68
isr 69
isr 70
isr 71
isr 72
isr 73
isr 74
isr 75
isr 76
isr 77
isr 78
isr 79
isr 80
isr 81
isr 82
isr 83
isr 84
isr 85
isr 86
isr 87
isr 88
isr 89
isr 90
isr 91
isr 92
isr 93
isr 94
isr 95
isr 96
isr 97
isr 98
isr 99
isr 100
isr 101
isr 102
isr 103
isr 104
isr 105
isr 106
isr 107
isr 108
isr 109
isr 110
isr 111
isr 112
isr 113
isr 114
isr 115
isr 116
isr 117
isr 118
isr 119
isr 120
isr 121
isr 122
isr 123
isr 124
isr 125
isr 126
isr 127
isr 128
isr 129
isr 130
isr 131
isr 132
isr 133
isr 134
isr 135
isr 136
isr 137
isr 138
isr 139
isr 140
isr 141
isr 142
isr 143
isr 144
isr 145
isr 146
isr 147
isr 148
isr 149
isr 150
isr 151
isr 152
isr 153
isr 154
isr 155
isr 156
isr 157
isr 158
isr 159
isr 160
isr 161
isr 162
isr 163
isr 164
isr 165
isr 166
isr 167
isr 168
isr 169
isr 170
isr 171
isr 172
isr 173
isr 174
isr 175
isr 176
isr 177
isr 178
isr 179
isr 180
isr 181
isr 182
isr 183
isr 184
isr 185
isr 186
isr 187
isr 188
isr 189
isr 190
isr 191
isr 192
isr 193
isr 194
isr 195
isr 196
isr 197
isr 198
isr 199
isr 200
isr 201
isr 202
isr 203
isr 204
isr 205
isr 206
isr 207
isr 208
isr 209
isr 210
isr 211
isr 212
isr 213
isr 214
isr 215
isr 216
isr 217
isr 218
isr 219
isr 220
isr 221
isr 222
isr 223
isr 224
isr 225
isr 226
isr 227
isr 228
isr 229
isr 230
isr 231
isr 232
isr 233
isr 234
isr 235
isr 236
isr 237
isr 238
isr 239
isr 240
isr 241
isr 242
isr 243
isr 244
isr 245
isr 246
isr 247
isr 248
isr 249
isr 250
isr 251
isr 252
isr 253
isr 254
isr 255
