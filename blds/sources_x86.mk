# List common C source files here.

DASDIR ?= ../dassys
DAPMPATH ?= $(DASDIR)/dapm05
DIDHPATH ?= $(DASDIR)/didh03
FILAPATH ?= $(DASDIR)/fila
MOPLPATH ?= $(DASDIR)/mopl03
VCTLPATH ?= $(DASDIR)/vctl04
COLAPATH ?= $(DASDIR)/cola
VDILPATH ?= $(DASDIR)/vdil03
SUPPPATH = $(DASDIR)/supp02
OBJDPATH = $(DASDIR)/objd04
# List common C source files here.
CSRC += \
	$(SUPPPATH)/eitp/eitp.c \
	$(SUPPPATH)/eitp/eitpcan2.c \
	../target_env/cami/eitp/eitp_cfg.c \
	../target_env/cami/objd_adapter/objd_eitp.c \
	$(DASDIR)/serl/gen_out/serl_das_didh_typ.c \
	$(DASDIR)/serl/gen_out/serl_das_objc.c \
	$(DASDIR)/serl/gen_out/serl_das_objc_cam_inp.c \
	$(DASDIR)/serl/gen_out/serl_das_gl2d.c \
	$(DASDIR)/serl/serl_dstd.c

CXXSRC += \
	../$(TARGET_ARCH)_env/vs6ether/udp_receiver.cpp \
	../$(TARGET_ARCH)_env/vs6ether/udp_sender.cpp \
	../$(TARGET_ARCH)_env/vs6ether/Vs6Ether.cpp \
	../$(TARGET_ARCH)_env/vs6ether/Vs6CanIface.cpp 

EXTRAINCDIRS +=  $(SUPPPATH)/gnss
EXTRAINCDIRS += ../target_env/dstd \
	../common_env \
	../x86_env/common \
	$(DASDIR)/odot03/odoc \
	$(SUPPPATH)/gl2d \
	$(SUPPPATH)/mtli \
	$(SUPPPATH)/errt02 \
	$(SUPPPATH)/capp \
	$(SUPPPATH)/vmcl \
	$(DIDHPATH) \
	$(COLAPATH)/colc \
	$(VDILPATH)/vdic \
	$(OBJDPATH)/objc \
	$(OBJDPATH)/objm \
	$(DASDIR)/ulfx/ulfc \
	$(SUPPPATH)/gmap \
	$(SUPPPATH)/rutm \
	$(SUPPPATH)/vegl \
	$(DAPMPATH) \
	$(DAPMPATH)/dapm_fun \
	$(VCTLPATH)/vctc \
	$(MOPLPATH)/mopc \
	$(FILAPATH)//filc
LIBS += \
	../$(TARGET_ARCH)_env/vs6ether/Lib/vxlapi.lib
	
DEFINES += -DXERRT_STOP_CRITICAL=FALSE -DXMTLI_HW=MTLI_32BIT_HW