# Final program directory
TARGET_DIR = ./bin
# Final program name
TARGET = Shell
# Header files directory
HEADER_DIR = .
# Header files
HEADERS = $(wildcard $(HEADER_DIR)/*.h)
# Source files directory
SOURCE_DIR = .
# Source files
SOURCES  = $(wildcard $(SOURCE_DIR)/*.c))
# Object files directory
OBJECT_DIR = ./obj
# Add prefix to the directory, substitue any .c with .o for all .c files.
OBJECTS = $(patsubst $(SOURCE_DIR)/%.c,$(OBJECT_DIR)/%.o,$(wildcard $(SOURCE_DIR)/*.c))
# Compiler to use.
COMPILER = gcc
# Include headers compiler flag.
IHCF = -I $(HEADER_DIR)
# Default action.
all: directories $(TARGET)
# Print any variable.
print-%  :
	@echo $* = $($*)
# Check our directory.
directories:
	# make directory if it isn't available.
	-mkdir -p $(OBJECT_DIR)
	# Make directory if it isn't available.
	-mkdir -p $(TARGET_DIR)
# Check each object file we have.
# If its equivalent .c file changed or any of the header file changed
# Reproduce the object file.
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c $(HEADERS)
	# $@ left side of condition, $< first operand of the right side
	$(COMPILER) -c -o $@ $< $(ICHF)
# Tell make to not delete these files as we need them even though objects are
# intermediary files.
.PRECIOUS: $(TARGET) $(OBJECTS)
# Check for any change in object, if there is a change reproduce the target file.
$(TARGET): $(OBJECTS)
	# Compile the final program.
	$(COMPILER) -o $(TARGET_DIR)/$@ $^ $(ICHF)
# Add option to make, if option entered it will do the suitable action.
.PHONY: clean
# Delete all generated files by make.
clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET_DIR)/$(TARGET)

# C code make file made by Amr Nasr.

