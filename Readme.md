### Build options

- `TT_BUILD_TESTS` - включает тесты в сборку проекта

**@NOTE**
- перед сборкой добавьте в 3rdparty_config.cmake:
	- `BOOST_INCLUDE` - директория где установлена библиотека boost
	- `SSL_INSTALL_PATH` - директория где установлена openss библиотека (или другая, но тогда еще нужно скорректировать fing_package(OponSSL)  
  		для вашей библиотеки в 3rdparty.cmake) eсли используете OpenSSL укажите USE_OPEN_SSL как ON




