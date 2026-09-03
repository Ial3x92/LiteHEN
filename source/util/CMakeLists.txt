cmake_minimum_required (VERSION 3.20)

set(basename "util")

project(${basename} C CXX ASM)

onion_module_prologue(C_STANDARD 11 CXX_STANDARD 20)

set(D_CWD "${CMAKE_CURRENT_SOURCE_DIR}")
onion_output_dirs()
set(D_SRC ${D_CWD}/source)

add_executable(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "util.elf")

target_compile_options(${PROJECT_NAME} PUBLIC
	$<$<COMPILE_LANGUAGE:C>:${C_DEFS} ${C_FLAGS}>
	$<$<COMPILE_LANGUAGE:CXX>:${CXX_DEFS} ${CXX_FLAGS}>
	$<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>
)

# MODIFICA: Rimossa la cartella 'sync' per escludere i file di download online
file(GLOB SrcFiles CONFIGURE_DEPENDS
  ${D_SRC}/*.c
  ${D_SRC}/*.cpp
  ${D_SRC}/cheats/*.c
  ${D_SRC}/cheats/*.cpp
  ${ONIONHEN_THIRD_PARTY_DIR}/cheat_support/*.c
  ${D_SRC}/../include/*.h
  ${D_SRC}/../include/*.hpp
  ${D_SRC}/*.s
  ${ONIONHEN_THIRD_PARTY_DIR}/cjson/*.cpp
  ${ONIONHEN_THIRD_PARTY_DIR}/cjson/*.hpp
)


# The PS5 payload SDK does not ship libsqlite3, so the public-domain
# amalgamation is vendored and compiled like the rest of third_party.
set(ONION_SQLITE_SOURCES "${ONIONHEN_THIRD_PARTY_DIR}/sqlite/sqlite3.c")

set(CMAKE_C_FLAGS "${ONION_PS5_TARGET_FLAGS} ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-keyword-macro -fPIC -fPIE -march=znver2 -Wall -Werror -fstack-protector-all -Wno-unused-const-variable -Wno-deprecated-declarations")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -gfull -gdwarf-2 -pedantic -pedantic-errors")

# MODIFICA: Rimossa la variabile ${SHADOWMOUNT_SOURCES} dalle sorgenti del target
target_sources(${PROJECT_NAME} PRIVATE ${SrcFiles} ${FTPSRV_SOURCES}
  ${ONION_SQLITE_SOURCES}
  "${ONIONHEN_COMMON_DIR}/elfldr_remote.c")
onion_target_debug_define(${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PRIVATE
  "${D_CWD}/include"
  "${ONIONHEN_THIRD_PARTY_DIR}/cjson"
  # miniz / sha256 / mc4 headers live beside their vendored .c files.
  "${ONIONHEN_THIRD_PARTY_DIR}/cheat_support"
  "${ONIONHEN_THIRD_PARTY_DIR}/keystone/include"
  "${PROJECT_ROOT}/libhijacker/include"
  "${PROJECT_ROOT}/libonion_settings/include"
  "${PROJECT_ROOT}/libonion_ipc/include"
  "${PROJECT_ROOT}/libonion_proc/include"
  "${PROJECT_ROOT}/libonion_platform/include"
  "${PROJECT_ROOT}/libonion_payload/include"
  "${PROJECT_ROOT}/libonion_elfldr/include"
  "${PROJECT_ROOT}/include"
)
# CBC for MC4 / ShnExt AES
target_compile_definitions(${PROJECT_NAME} PRIVATE CBC=1)

# DISABILITATO: Rimosso il controllo di libcurl per il catalogo online
# set(ONION_CURL_ROOT "${PS5_PAYLOAD_SDK}/target/user/homebrew")
# set(ONION_CURL_CA_BUNDLE "${ONION_CURL_ROOT}/etc/ca-bundle.crt")
# if(NOT EXISTS "${ONION_CURL_ROOT}/include/curl/curl.h" OR
#    NOT EXISTS "${ONION_CURL_ROOT}/lib/libcurl.a" OR
#    NOT EXISTS "${ONION_CURL_CA_BUNDLE}")
# 	message(FATAL_ERROR
# 		"PS5 payload SDK libcurl and CA bundle are required for cheat catalog HTTP "
# 		"(looked in ${ONION_CURL_ROOT}).")
# endif()

# MODIFICA: Rimosse tutte le definizioni di precompilazione legate a SHADOWMOUNT
target_compile_definitions(${PROJECT_NAME} PRIVATE
	CURL_STATICLIB
	ONION_CURL_CA_BUNDLE="${ONION_CURL_CA_BUNDLE}"
  VERSION_TAG="OnionHEN ftpsrv module"
)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
	"${ONION_CURL_ROOT}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${FTPSRV_SOURCE_DIR}")

# MODIFICA: Rimosse le directory di inclusione del modulo shadowmount (${SHADOWMOUNT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE
  "${ONIONHEN_THIRD_PARTY_DIR}/sqlite")

# DISABILITATO: Dipendenza dal file del certificato CA rimossa
# set_source_files_properties(
# 	"${D_SRC}/cheats/sync/http_transport_ps5.cpp"
# 	PROPERTIES OBJECT_DEPENDS "${ONION_CURL_CA_BUNDLE}"
# )

# Upstream ftpsrv is compiled with a less strict warning profile than the
# OnionHEN util sources.  Keep the shared target's -Werror for our code while
# allowing the pinned third-party modules to retain their upstream
# implementation.
set_source_files_properties(${FTPSRV_SOURCES} PROPERTIES
  COMPILE_FLAGS "-Wno-error -Wno-unused-parameter -Wno-sign-compare")

# MODIFICA: Rimossa la variabile ${SHADOWMOUNT_SOURCES} dalle proprietà dei file sorgente
set_source_files_properties(${ONION_SQLITE_SOURCES}
  PROPERTIES COMPILE_FLAGS
  "-Wno-error -Wno-unused-parameter -Wno-sign-compare -Wno-unused-function")
target_link_directories(${PROJECT_NAME} PUBLIC
  "${ONIONHEN_PS5_STUB_DIR}"
  "${ONIONHEN_THIRD_PARTY_DIR}/keystone/lib"
  "${ONION_CURL_ROOT}/lib"
  "${PS5_PAYLOAD_SDK}/target/lib"
)
add_dependencies(${PROJECT_NAME} hijacker onion_settings onion_ipc onion_proc onion_platform onion_payload onion_ready onion_elfldr)
# keystone: ShnExt ASM only
target_link_libraries(${PROJECT_NAME} PUBLIC
    onion_ipc
    onion_settings
    onion_proc
    onion_platform
    onion_payload
    onion_ready
    onion_elfldr
    keystone
    hijacker
    SceSystemService
    SceUserService
    SceAppInstUtil
    SceNotification
    SceNetCtl
    SceNet
    kernel_sys
    # MODIFICA: Rimosse le librerie zstd e z che causavano l'errore del linker
)
set_source_files_properties(
  ${ONIONHEN_THIRD_PARTY_DIR}/cheat_support/aes.c
  ${ONIONHEN_THIRD_PARTY_DIR}/cheat_support/base64.c
  ${ONIONHEN_THIRD_PARTY_DIR}/cheat_support/miniz.c
  ${ONIONHEN_THIRD_PARTY_DIR}/cheat_support/sha256.c
  PROPERTIES COMPILE_FLAGS "-Wno-error -Wno-unused-function -Wno-sign-compare -Wno-unused-parameter"
)

# DISABILITATO: Rimosso il file zip_archive poiché legato alle funzioni sync
# set_source_files_properties(
#   ${D_SRC}/cheats/sync/zip_archive.cpp
#   PROPERTIES COMPILE_FLAGS "-Wno-error -Wno-unused-function -Wno-sign-compare -Wno-unused-parameter"
# )

file(GLOB CHEAT_ENGINE_SRCS CONFIGURE_DEPENDS ${D_SRC}/cheats/*.c)
set_source_files_properties(${CHEAT_ENGINE_SRCS} PROPERTIES
  COMPILE_FLAGS "-Wno-error -Wno-unused-parameter -Wno-sign-compare"
)

onion_strip_debug(${PROJECT_NAME})
