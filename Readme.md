### Build options

- `TT_BUILD_TESTS` - включает тесты в сборку проекта. По умолчанию `ON`
- `TT_EXTERNAL_UPLOAD_SOURCE_DIR` - добавить для тестов путь, который будет указан как параметр  
  запуска сервера. Без этой опции будет будет использовать `./resource/upload` от корневой  
  директории проекта
- `TT_EXTERNAL_DOWNLOAD_SOURCE_DIR` - добавить путь, куда будут скачиваться файлы, по умолчаниб  
  будет использоваться  `./resource/downloads` от корневой директории проекта

**Примеры вызовов cmake**
```bash
$ cmake -G Ninja -S ./ -B build/linux -DTT_EXTERNAL_UPLOAD_SOURCE_DIR=path-to-dir -DTT_EXTERNAL_UPLOAD_SOURCE_DIR=path-to-dir
```

**@NOTE**
- перед сборкой добавьте в 3rdparty_config.cmake:
	- `BOOST_INCLUDE` - директория где установлена библиотека boost
	- `SSL_INSTALL_PATH` - директория где установлена openss библиотека (или другая, но тогда еще нужно скорректировать fing_package(OponSSL)  
  		для вашей библиотеки в 3rdparty.cmake) eсли используете OpenSSL укажите USE_OPEN_SSL как ON




