# C++ Lab Configuration Files

Never use this is repository as a submodule. It is mean to serve as a repository for sample-configurations and fixed-configurations.

A fixed-configuration is a configuration that is set at the start of the term and does not change for the duration of the term, unless there exists a bug, an error, or an urgent feature addition. These files can be compied without being modified.

A sample-configuration is copied into a lab and modified to fit the requirements of the lab.

* `clang_tidy_config.py` (fixed) The clang-tidy configuration for used for linting
* `makefile_config.py` (fixed) GNU Makefiles are built from this configuration
* `old_lab_config.py` (deprecated) An example of the old-style combined lab_config.py file.
* `sample_lab_config.py` (sample) A new-style lab configuration file.
* `sample_system_test.py` (sample) An example of how a system_test.py can be written
