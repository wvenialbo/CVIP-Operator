mkdir source
mkdir source\config
mkdir source\core

mklink /H source\config\config.hpp ..\CVIP\source\config\config.hpp
mklink /H source\config\platform.hpp ..\CVIP\source\config\platform.hpp
mklink /H source\config\basic_types.hpp ..\CVIP\source\config\basic_types.hpp
mklink /H source\config\basic_imports.hpp ..\CVIP\source\config\basic_imports.hpp

mklink /H source\core\i_operator.hpp ..\CVIP\source\core\i_operator.hpp

mklink /H source\core\operator.hpp ..\CVIP\source\core\operator.hpp
mklink /H source\core\operator.inl ..\CVIP\source\core\operator.inl
mklink /H source\core\operator.cpp ..\CVIP\source\core\operator.cpp

mklink /H source\core\expression.hpp ..\CVIP\source\core\expression.hpp
mklink /H source\core\expression.inl ..\CVIP\source\core\expression.inl
mklink /H source\core\expression.cpp ..\CVIP\source\core\expression.cpp


mkdir tests
mkdir tests\common
mkdir tests\core

mklink /H tests\common\main.cpp ..\CVIP\tests\common\main.cpp
mklink /H tests\core\operator.cpp ..\CVIP\tests\core\operator.cpp
