AR = ['arm-none-eabi-ar']
ARFLAGS = ['rcs']
AS = 'arm-none-eabi-gcc'
BINDIR = '/usr/local/bin'
BLOCK_MESSAGE_KEYS = []
BUILD_DIR = 'emery'
BUILD_TYPE = 'app'
BUNDLE_BIN_DIR = 'emery'
BUNDLE_NAME = 'WVA-M640D.pbw'
CC = ['arm-none-eabi-gcc']
CCLNK_SRC_F = []
CCLNK_TGT_F = ['-o']
CC_NAME = 'gcc'
CC_SRC_F = []
CC_TGT_F = ['-c', '-o']
CC_VERSION = ('14', '2', '1')
CFLAGS = ['-std=c99', '-mcpu=cortex-m3', '-mthumb', '-ffunction-sections', '-fdata-sections', '-fcommon', '-g', '-fPIE', '-Os', '-D_TIME_H_', '-Dtime_t=long', '-Wall', '-Wextra', '-Werror', '-Wno-unused-parameter', '-Wno-error=unused-function', '-Wno-error=unused-variable', '-Wno-error=builtin-declaration-mismatch', '-Wno-error=format-truncation', '-Wno-error=expansion-to-defined', '-Wno-error=zero-length-bounds', '-Wno-error=cast-function-type', '-Wno-error=unused-value']
CFLAGS_MACBUNDLE = ['-fPIC']
CFLAGS_cshlib = ['-fPIC']
CPPPATH_ST = '-I%s'
DEFINES = ['RELEASE', 'PBL_PLATFORM_EMERY', 'PBL_COLOR', 'PBL_RECT', 'PBL_MICROPHONE', 'PBL_SMARTSTRAP', 'PBL_HEALTH', 'PBL_SMARTSTRAP_POWER', 'PBL_COMPASS', 'PBL_TOUCH', 'PBL_RGB_BACKLIGHT', 'PBL_SPEAKER', 'PBL_DISPLAY_WIDTH=200', 'PBL_DISPLAY_HEIGHT=228', 'PBL_SDK_3']
DEFINES_ST = '-D%s'
DEST_BINFMT = 'elf'
DEST_CPU = 'arm'
DEST_OS = 'linux'
INCLUDES = ['emery']
LD = 'arm-none-eabi-ld'
LIBDIR = '/usr/local/lib64'
LIBPATH_ST = '-L%s'
LIB_DIR = 'node_modules'
LIB_JSON = [{'name': 'pebble-clay', 'version': '1.0.4', 'description': 'Pebble Config Framework', 'scripts': {'test-travis': './node_modules/.bin/gulp && ./node_modules/.bin/karma start ./test/karma.conf.js --single-run --browsers chromeTravisCI && ./node_modules/.bin/eslint ./', 'test-debug': '(export DEBUG=true && ./node_modules/.bin/gulp && ./node_modules/.bin/karma start ./test/karma.conf.js --no-single-run)', 'test': './node_modules/.bin/gulp && ./node_modules/.bin/karma start ./test/karma.conf.js --single-run', 'lint': './node_modules/.bin/eslint ./', 'build': 'gulp', 'dev': 'gulp dev', 'pebble-clean': 'rm -rf tmp src/js/index.js && pebble clean', 'pebble-publish': 'npm run pebble-clean && npm run build && pebble build && pebble package publish && npm run pebble-clean', 'pebble-build': 'npm run build && pebble build'}, 'repository': {'type': 'git', 'url': 'git+https://github.com/pebble/clay.git'}, 'keywords': ['pebble', 'config', 'configuration', 'pebble-package'], 'author': 'Pebble Technology', 'license': 'MIT', 'bugs': {'url': 'https://github.com/pebble/clay/issues'}, 'pebble': {'projectType': 'package', 'sdkVersion': '3', 'targetPlatforms': ['aplite', 'basalt', 'chalk', 'diorite', 'emery'], 'resources': {'media': []}, 'capabilities': ['configurable']}, 'homepage': 'https://github.com/pebble/clay#readme', 'devDependencies': {'autoprefixer': '^6.3.1', 'bourbon': '^4.2.6', 'browserify': '^13.0.0', 'browserify-istanbul': '^0.2.1', 'chai': '^3.4.1', 'deamdify': '^0.2.0', 'deepcopy': '^0.6.1', 'del': '^2.0.2', 'eslint': '^1.5.1', 'eslint-config-pebble': '^1.2.0', 'eslint-plugin-standard': '^1.3.1', 'gulp': '^3.9.0', 'gulp-autoprefixer': '^3.1.0', 'gulp-htmlmin': '^1.3.0', 'gulp-inline': '0.0.15', 'gulp-insert': '^0.5.0', 'gulp-sass': '^2.1.1', 'gulp-sourcemaps': '^1.6.0', 'gulp-uglify': '^1.5.2', 'joi': '^6.10.1', 'karma': '^0.13.19', 'karma-browserify': '^5.0.1', 'karma-chrome-launcher': '^0.2.2', 'karma-coverage': '^0.5.3', 'karma-mocha': '^0.2.1', 'karma-mocha-reporter': '^1.1.5', 'karma-source-map-support': '^1.1.0', 'karma-threshold-reporter': '^0.1.15', 'mocha': '^2.3.4', 'postcss': '^5.0.14', 'require-from-string': '^1.1.0', 'sassify': '^0.9.1', 'sinon': '^1.17.3', 'stringify': '^3.2.0', 'through': '^2.3.8', 'tosource': '^1.0.0', 'vinyl-buffer': '^1.0.0', 'vinyl-source-stream': '^1.1.0', 'watchify': '^3.7.0'}, 'dependencies': {}, 'path': 'node_modules/pebble-clay/dist', 'js_paths': ['node_modules/pebble-clay/dist/js/index.js']}]
LIB_RESOURCES_JSON = {'pebble-clay': []}
LIB_ST = '-l%s'
LINKFLAGS = ['-mcpu=cortex-m3', '-mthumb', '-Wl,--gc-sections', '-Wl,--warn-common', '-fPIE', '-Os']
LINKFLAGS_MACBUNDLE = ['-bundle', '-undefined', 'dynamic_lookup']
LINKFLAGS_cshlib = ['-shared']
LINKFLAGS_cstlib = ['-Wl,-Bstatic']
LINK_CC = ['arm-none-eabi-gcc']
MESSAGE_KEYS = {'JS_READY': 10000, 'W_ICON': 10001, 'W_TEMP': 10002, 'W_COND': 10003, 'S_UNITS': 10004, 'S_VIB_BT': 10005, 'S_BATT_DISPLAY': 10006, 'OWM_API_KEY': 10007}
MESSAGE_KEYS_DEFINITION = '/home/carlosr/personal/WVA-M640D/build/src/message_keys.auto.c'
MESSAGE_KEYS_HEADER = '/home/carlosr/personal/WVA-M640D/build/include/message_keys.auto.h'
MESSAGE_KEYS_JSON = '/home/carlosr/personal/WVA-M640D/build/js/message_keys.json'
NODE_PATH = '/home/carlosr/.pebble-sdk/SDKs/current/node_modules'
PEBBLE_SDK_COMMON = '/home/carlosr/.pebble-sdk/SDKs/current/sdk-core/pebble/common'
PEBBLE_SDK_PLATFORM = '/home/carlosr/.pebble-sdk/SDKs/current/sdk-core/pebble/emery'
PEBBLE_SDK_ROOT = '/home/carlosr/.pebble-sdk/SDKs/current/sdk-core/pebble'
PLATFORM = {'NAME': 'emery', 'MAX_APP_BINARY_SIZE': 131072, 'MAX_APP_MEMORY_SIZE': 131072, 'MAX_WORKER_MEMORY_SIZE': 10240, 'MAX_RESOURCES_SIZE_APPSTORE': 262144, 'MAX_RESOURCES_SIZE': 1048576, 'DEFINES': ['PBL_PLATFORM_EMERY', 'PBL_COLOR', 'PBL_RECT', 'PBL_MICROPHONE', 'PBL_SMARTSTRAP', 'PBL_HEALTH', 'PBL_SMARTSTRAP_POWER', 'PBL_COMPASS', 'PBL_TOUCH', 'PBL_RGB_BACKLIGHT', 'PBL_SPEAKER', 'PBL_DISPLAY_WIDTH=200', 'PBL_DISPLAY_HEIGHT=228'], 'BUILD_DIR': 'emery', 'BUNDLE_BIN_DIR': 'emery', 'ADDITIONAL_TEXT_LINES_FOR_PEBBLE_H': [], 'MAX_FONT_GLYPH_SIZE': 512, 'TAGS': ['emery', 'color', 'rect', 'mic', 'strap', 'health', 'strappower', 'compass', 'touch', 'speaker', '200w', '228h']}
PLATFORM_NAME = 'emery'
PREFIX = '/usr/local'
PROJECT_INFO = {'displayName': 'WVA-M640D', 'uuid': '633b94e6-2a0d-4f78-8ad8-4c94c8918e24', 'sdkVersion': '3', 'enableMultiJS': True, 'targetPlatforms': ['emery'], 'watchapp': {'watchface': True}, 'messageKeys': {'JS_READY': 10000, 'W_ICON': 10001, 'W_TEMP': 10002, 'W_COND': 10003, 'S_UNITS': 10004, 'S_VIB_BT': 10005, 'S_BATT_DISPLAY': 10006, 'OWM_API_KEY': 10007}, 'resources': {'media': [{'type': 'bitmap', 'name': 'BACKGROUND', 'file': 'images/WVA-M640D.png', 'targetPlatforms': ['emery']}, {'type': 'bitmap', 'name': 'HOUR_HAND', 'file': 'images/WVA-M640D-hour.png'}, {'type': 'bitmap', 'name': 'MINUTE_HAND', 'file': 'images/WVA-M640D-minute.png'}, {'type': 'bitmap', 'name': 'SECOND_HAND', 'file': 'images/WVA-M640D-second.png'}, {'type': 'bitmap', 'name': 'BATTERY', 'file': 'images/Battery.png'}, {'characterRegex': '[-+ACDEFHIMNOSTUW:0-9\xb0%]', 'type': 'font', 'name': 'DOTO_18', 'file': 'fonts/Doto-VariableFont_ROND,wght.ttf'}, {'characterRegex': '[-+ACDEFHIMNOSTUW:0-9\xb0%]', 'type': 'font', 'name': 'DOTO_16', 'file': 'fonts/Doto-VariableFont_ROND,wght.ttf'}, {'characterRegex': '[I"!k$+F9=N#,>h]', 'type': 'font', 'name': 'WEATHER_ICON_24', 'file': 'fonts/Climacons.ttf'}]}, 'name': 'WVA-M640D', 'shortName': 'WVA-M640D', 'longName': 'WVA-M640D', 'versionLabel': '1.0.0', 'companyName': 'MakeAwesomeHappen', 'appKeys': {'JS_READY': 10000, 'W_ICON': 10001, 'W_TEMP': 10002, 'W_COND': 10003, 'S_UNITS': 10004, 'S_VIB_BT': 10005, 'S_BATT_DISPLAY': 10006, 'OWM_API_KEY': 10007}}
REQUESTED_PLATFORMS = ['emery']
RESOURCES_JSON = [{'type': 'bitmap', 'name': 'BACKGROUND', 'file': 'images/WVA-M640D.png', 'targetPlatforms': ['emery']}, {'type': 'bitmap', 'name': 'HOUR_HAND', 'file': 'images/WVA-M640D-hour.png'}, {'type': 'bitmap', 'name': 'MINUTE_HAND', 'file': 'images/WVA-M640D-minute.png'}, {'type': 'bitmap', 'name': 'SECOND_HAND', 'file': 'images/WVA-M640D-second.png'}, {'type': 'bitmap', 'name': 'BATTERY', 'file': 'images/Battery.png'}, {'characterRegex': '[-+ACDEFHIMNOSTUW:0-9\xb0%]', 'type': 'font', 'name': 'DOTO_18', 'file': 'fonts/Doto-VariableFont_ROND,wght.ttf'}, {'characterRegex': '[-+ACDEFHIMNOSTUW:0-9\xb0%]', 'type': 'font', 'name': 'DOTO_16', 'file': 'fonts/Doto-VariableFont_ROND,wght.ttf'}, {'characterRegex': '[I"!k$+F9=N#,>h]', 'type': 'font', 'name': 'WEATHER_ICON_24', 'file': 'fonts/Climacons.ttf'}]
RPATH_ST = '-Wl,-rpath,%s'
SANDBOX = False
SDK_VERSION_MAJOR = 5
SDK_VERSION_MINOR = 95
SHLIB_MARKER = None
SIZE = 'arm-none-eabi-size'
SONAME_ST = '-Wl,-h,%s'
STLIBPATH_ST = '-L%s'
STLIB_MARKER = None
STLIB_ST = '-l%s'
SUPPORTED_PLATFORMS = ['basalt', 'aplite', 'diorite', 'chalk', 'flint', 'emery', 'gabbro']
TARGET_PLATFORMS = ['emery']
TIMESTAMP = 1777828878
USE_GROUPS = True
VERBOSE = 0
WEBPACK = ['/home/carlosr/.pebble-sdk/SDKs/current/node_modules/.bin/webpack']
cprogram_PATTERN = '%s'
cshlib_PATTERN = 'lib%s.so'
cstlib_PATTERN = 'lib%s.a'
macbundle_PATTERN = '%s.bundle'
