.PHONY: create-%, run-%

# Variable
m = "Update"

help:
	@echo "To push all changes with commit message "Update" type: 'make push'"
	@echo "To push all changes with personal commit message type: 'make push m="Your commit message"'"

# Rule to create files
create-%:
	@touch $*.h $*.cpp

# Rule to compile and run
#run: 
#	clang++ -std=c++17  -o a
#	@./a
#	@rm a

push:
	git add .
	git commit -m "$(m)"
	git push