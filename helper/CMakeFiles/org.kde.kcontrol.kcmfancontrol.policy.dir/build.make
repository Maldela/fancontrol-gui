# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maldela/Fancontrol-GUI

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maldela/Fancontrol-GUI

# Utility rule file for org.kde.kcontrol.kcmfancontrol.policy.

# Include the progress variables for this target.
include helper/CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy.dir/progress.make

helper/CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy: helper/org.kde.kcontrol.kcmfancontrol.policy
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maldela/Fancontrol-GUI/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "actions for org.kde.kcontrol.kcmfancontrol"

helper/org.kde.kcontrol.kcmfancontrol.policy: /usr/lib/kauth/kauth-policy-gen
helper/org.kde.kcontrol.kcmfancontrol.policy: helper/kcmfancontrol.actions
	$(CMAKE_COMMAND) -E cmake_progress_report /home/maldela/Fancontrol-GUI/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating org.kde.kcontrol.kcmfancontrol.policy"
	cd /home/maldela/Fancontrol-GUI/helper && /usr/lib/kauth/kauth-policy-gen /home/maldela/Fancontrol-GUI/helper/kcmfancontrol.actions /home/maldela/Fancontrol-GUI/helper/org.kde.kcontrol.kcmfancontrol.policy

org.kde.kcontrol.kcmfancontrol.policy: helper/CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy
org.kde.kcontrol.kcmfancontrol.policy: helper/org.kde.kcontrol.kcmfancontrol.policy
org.kde.kcontrol.kcmfancontrol.policy: helper/CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy.dir/build.make
.PHONY : org.kde.kcontrol.kcmfancontrol.policy

# Rule to build all files generated by this target.
helper/CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy.dir/build: org.kde.kcontrol.kcmfancontrol.policy
.PHONY : helper/CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy.dir/build

helper/CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy.dir/clean:
	cd /home/maldela/Fancontrol-GUI/helper && $(CMAKE_COMMAND) -P CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy.dir/cmake_clean.cmake
.PHONY : helper/CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy.dir/clean

helper/CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy.dir/depend:
	cd /home/maldela/Fancontrol-GUI && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maldela/Fancontrol-GUI /home/maldela/Fancontrol-GUI/helper /home/maldela/Fancontrol-GUI /home/maldela/Fancontrol-GUI/helper /home/maldela/Fancontrol-GUI/helper/CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : helper/CMakeFiles/org.kde.kcontrol.kcmfancontrol.policy.dir/depend
