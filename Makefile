include nemu/Makefile.git

# Working spaces for nemu
V_CODE = code $(NEMU_HOME)
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
	@$(addprefix $(V_CODE)/, include)
	@$(addprefix $(V_CODE)/, src)
	@$(addprefix $(V_CODE)/, src/isa/x86)
	@$(V_CODE)  #(tools)

.PHONY: default clean submit vnemu
