LIB =	cstdmf

SRCS =					\
	base64				\
	bwversion			\
	binary_stream		\
	concurrency			\
	debug				\
	diary				\
	dogwatch			\
	dprintf				\
	intrusive_object	\
	md5					\
	profile				\
	stpwatch			\
	timestamp			\
	watcher				\

ifndef MF_ROOT
export MF_ROOT := $(subst /src/lib/$(LIB),,$(CURDIR))
endif

include $(MF_ROOT)/src/lib/makestuf/stdmake.mak

