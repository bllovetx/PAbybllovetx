include nemu/Makefile.git

# Working spaces for vscode
V_NEMU = code $(NEMU_HOME)
V_AM= code $(AM_HOME)
#NEMU_WS = include src src/isa/x86 tools
#NEMU_WSDIR = $(addprefix $(NEMU_HOME)/, $(NEMU_WS))

default:
	@echo "Please run 'make' under any subprojects to compile."
 
clean:
	-$(MAKE) -C nemu clean
	-$(MAKE) -C nexus-am clean
	-$(MAKE) -C nanos-lite clean
	-$(MAKE) -C navy-apps clean

submit: clean
	git gc
	STUID=$(STUID) STUNAME=$(STUNAME) bash -c "$$(curl -s http://ics.nju.edu.cn/people/yanyanjiang/teach/submit.sh)"

vnemu:
	@$(addprefix $(V_NEMU)/, include)
	@$(addprefix $(V_NEMU)/, src)
	@$(addprefix $(V_NEMU)/, src/isa/x86)
	@$(V_NEMU)  #(tools)

vam:
	@$(addprefix $(V_AM)/, libs/klib)
	@$(addprefix $(V_AM)/, am)
	@$(addprefix $(V_AM)/, apps)
	@$(V_AM)  #(tools)

.PHONY: default clean submit vnemu vam
