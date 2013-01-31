ShellClose: ShellClose.cpp
	$(CXX) $^ -o $@

.PHONY clean:
	$(RM) ShellClose
