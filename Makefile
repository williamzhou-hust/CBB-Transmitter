#   BSD LICENSE
#
#   Copyright(c) 2010-2014 Intel Corporation. All rights reserved.
#   All rights reserved.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in
#       the documentation and/or other materials provided with the
#       distribution.
#     * Neither the name of Intel Corporation nor the names of its
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
#   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
export RTE_SDK ?=$(CURDIR)/dpdk-stable-16.11.1
ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif

# Default target, can be overridden by command line or environment
RTE_TARGET ?= x86_64-native-linuxapp-gcc

include $(RTE_SDK)/mk/rte.vars.mk

# binary name
APP = CBB-Transmiter

VPATH += $(SRCDIR)/BCCencode
#VPATH += $(SRCDIR)/IFFT
VPATH += $(SRCDIR)/intrinsics_interface
VPATH += $(SRCDIR)/Process
VPATH += $(SRCDIR)/process_data
VPATH += $(SRCDIR)/process_data/process_datafunction
VPATH += $(SRCDIR)/typeDef
VPATH += $(SRCDIR)/VarINIT

# all source are stored in SRCS-y
SRCS-y := main.c mainbfBCC.c test.c maindpdk.c maindpdk2.c maindpdk3.c
SRCS-y += ccoding_byte.c

#SRCS-y += ifft.c ifftShiftandIFFTPreamble.c

SRCS-y += intrinsics_interface_v2.c
SRCS-y += generatePreambleAndHeLTF_csd.c
SRCS-y += bccInterleaverForSig.c csdForHeLTF.c csdForPreamble.c
SRCS-y += generateBasicSig.c generateHeLTF_csd.c generateLTF_csd.c
SRCS-y += generatePreamble_csd.c generateSig_csd.c generateSTF_csd.c
SRCS-y += MapForSig.c setSigInfo.c
           
SRCS-y += mcs_table_for_20M.c process_data.c
SRCS-y += BCC_encoder.c Data_CSD.c GenDataAndScramble.c
SRCS-y += GenInit.c modulate.c modulate_opt.c PilotAdd_SubcarMap.c

SRCS-y += commonStructure.c

SRCS-y += globalVarINIT.c

CFLAGS += -O1
CFLAGS += -I$(SRCDIR)/BCCencode
#CFLAGS += -I$(SRCDIR)/IFFT
CFLAGS += -I$(SRCDIR)/intrinsics_interface
CFLAGS += -I$(SRCDIR)/Process
CFLAGS += -I$(SRCDIR)/process_data
CFLAGS += -I$(SRCDIR)/process_data/process_datafunction
CFLAGS += -I$(SRCDIR)/typeDef
CFLAGS += -I$(SRCDIR)/VarINIT

CFLAGS += -D OPTIMIZATION
CFLAGS += -D AVX2
#CFLAGS += $(WERROR_FLAGS)

#LDLIBS += -L$(subst main,print_abcd,$(RTE_OUTPUT))/lib
#LDLIBS += -L$(RTE_EXLIB_CBB)/BCCencode/$(RTE_TARGET)/lib
#LDLIBS += -L$(RTE_EXLIB_CBB)/IFFT/$(RTE_TARGET)/lib
#LDLIBS += -L$(RTE_EXLIB_CBB)/intrinsics_interface/$(RTE_TARGET)/lib
#LDLIBS += -L$(RTE_EXLIB_CBB)/Process/$(RTE_TARGET)/lib
#LDLIBS += -L$(RTE_EXLIB_CBB)/process_data/$(RTE_TARGET)/lib
#LDLIBS += -L$(RTE_EXLIB_CBB)/typeDef/$(RTE_TARGET)/lib
#LDLIBS += -L$(RTE_EXLIB_CBB)/VarINIT/$(RTE_TARGET)/lib

#LDLIBS += -lBCCencode
#LDLIBS += -lIFFT
#LDLIBS += -lintrinsics_interface
#LDLIBS += -lProcess
#LDLIBS += -lprocess_data
#LDLIBS += -ltypeDef
#LDLIBS += -lVarINIT
#$(info "Please $(RTE_OUTPUT)")
include $(RTE_SDK)/mk/rte.extapp.mk
