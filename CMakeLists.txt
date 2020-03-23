cmake_minimum_required(VERSION 3.14)

project(crashpad)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

enable_language(ASM_MASM)

add_subdirectory(vendor/zlib)

add_library(
    mini_chromium
    STATIC
    vendor/mini_chromium/base/logging
    vendor/mini_chromium/base/rand_util
    vendor/mini_chromium/base/files/file_path
    vendor/mini_chromium/base/files/scoped_file
    vendor/mini_chromium/base/process/memory
    vendor/mini_chromium/base/process/process_metrics_win
    vendor/mini_chromium/base/strings/stringprintf
    vendor/mini_chromium/base/strings/string_number_conversions
    vendor/mini_chromium/base/strings/string_util_win
    vendor/mini_chromium/base/strings/utf_string_conversion_utils
    vendor/mini_chromium/base/strings/utf_string_conversions
    vendor/mini_chromium/base/synchronization/lock
    vendor/mini_chromium/base/synchronization/lock_impl_win
    vendor/mini_chromium/base/third_party/icu/icu_utf
)

target_compile_definitions(
    mini_chromium
    PRIVATE
    -DNOMINMAX
    -DUNICODE
    -DWIN32
    -DWIN32_LEAN_AND_MEAN
)

target_include_directories(
    mini_chromium
    PUBLIC
    vendor/mini_chromium
)

add_library(
    getopt
    STATIC
    third_party/getopt/getopt
)

target_compile_definitions(
    getopt
    PRIVATE
    -DNOMINMAX
    -DUNICODE
    -DWIN32
    -DWIN32_LEAN_AND_MEAN
)

target_include_directories(
    getopt
    PUBLIC
    third_party/getopt
)

add_library(
    crashpad_compat
    STATIC
    compat/win/strings.cc
    compat/win/time.cc
    compat/win/time.h
)

target_compile_definitions(
    crashpad_compat
    PRIVATE
    -DNOMINMAX
    -DUNICODE
    -DWIN32_LEAN_AND_MEAN
)

target_include_directories(
    crashpad_compat
    PUBLIC
    compat/win
)

add_library(
    crashpad_tools
    STATIC
    tools/tool_support
)

target_compile_definitions(
    crashpad_tools
    PRIVATE
    -DNOMINMAX
    -DUNICODE
    -DWIN32_LEAN_AND_MEAN
)

target_link_libraries(
    crashpad_tools
    PUBLIC
    mini_chromium
)

add_library(
    crashpad_util
    STATIC
    util/file/directory_reader_win
    util/file/file_io
    util/file/file_io_win
    util/file/file_reader
    util/file/file_seeker
    util/file/file_writer
    util/file/filesystem_win
    util/file/scoped_remove_file
    util/misc/capture_context_win.asm
    util/misc/initialization_state_dcheck
    util/misc/metrics
    util/misc/paths_win
    util/misc/random_string
    util/misc/time_win
    util/misc/uuid
    util/misc/zlib
    util/net/http_body
    util/net/http_body_gzip
    util/net/http_multipart_builder
    util/net/http_transport
    util/net/http_transport_win
    util/net/url
    util/numeric/checked_address_range
    util/process/process_memory
    util/process/process_memory_range
    util/process/process_memory_win
    util/stdlib/aligned_allocator
    util/stdlib/string_number_conversion
    util/stdlib/strlcpy
    util/string/split_string
    util/synchronization/semaphore_win
    util/thread/thread
    util/thread/thread_win
    util/thread/worker_thread
    util/win/command_line
    util/win/critical_section_with_debug_info
    util/win/exception_handler_server
    util/win/get_function
    util/win/handle
    util/win/initial_client_data
    util/win/loader_lock
    util/win/module_version
    util/win/nt_internals
    util/win/ntstatus_logging
    util/win/process_info
    util/win/registration_protocol_win
    util/win/safe_terminate_process.asm
    util/win/session_end_watcher
    util/win/scoped_handle
    util/win/scoped_local_alloc
    util/win/scoped_process_suspend
    util/win/scoped_set_event
)

target_compile_definitions(
    crashpad_util
    PRIVATE
    -DCRASHPAD_ZLIB_SOURCE_EXTERNAL
    -DNOMINMAX
    -DUNICODE
    -DWIN32
    -DWIN32_LEAN_AND_MEAN
)

target_link_libraries(
    crashpad_util
    PRIVATE
    mini_chromium
    crashpad_compat
    zlibstatic

    user32
    version
    winhttp
)

target_include_directories(
    crashpad_util
    PUBLIC
    vendor/zlib
    "$<TARGET_FILE_DIR:zlibstatic>/../"
)

add_library(
    crashpad_client
    STATIC
    client/annotation
    client/annotation_list
    client/crash_report_database
    client/crash_report_database_win
    client/crashpad_client_win
    client/crashpad_info
    client/prune_crash_reports
    client/settings
)

target_compile_definitions(
    crashpad_client
    PRIVATE
    -DNOMINMAX
    -DUNICODE
)

target_include_directories(
    crashpad_client
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    crashpad_client
    PRIVATE
    mini_chromium
    crashpad_compat
    crashpad_util
    rpcrt4
)

add_library(
    crashpad_minidump
    STATIC
    minidump/minidump_annotation_writer
    minidump/minidump_byte_array_writer
    minidump/minidump_context_writer
    minidump/minidump_crashpad_info_writer
    minidump/minidump_exception_writer
    minidump/minidump_file_writer
    minidump/minidump_handle_writer
    minidump/minidump_memory_writer
    minidump/minidump_memory_info_writer
    minidump/minidump_misc_info_writer
    minidump/minidump_module_crashpad_info_writer
    minidump/minidump_module_writer
    minidump/minidump_rva_list_writer
    minidump/minidump_simple_string_dictionary_writer
    minidump/minidump_stream_writer
    minidump/minidump_string_writer
    minidump/minidump_system_info_writer
    minidump/minidump_thread_id_map
    minidump/minidump_thread_writer
    minidump/minidump_unloaded_module_writer
    minidump/minidump_user_stream_writer
    minidump/minidump_writable
    minidump/minidump_writer_util
)

target_compile_definitions(
    crashpad_minidump
    PRIVATE
    -DNOMINMAX
    -DUNICODE
    -DWIN32
    -DWIN32_LEAN_AND_MEAN
)

target_link_libraries(
    crashpad_minidump
    PRIVATE
    mini_chromium
    crashpad_compat
    crashpad_util
)

add_library(
    crashpad_snapshot
    STATIC
    snapshot/annotation_snapshot
    snapshot/capture_memory
    snapshot/cpu_context
    snapshot/crashpad_info_client_options
    snapshot/handle_snapshot
    snapshot/memory_snapshot
    snapshot/unloaded_module_snapshot
    snapshot/crashpad_types/crashpad_info_reader
    snapshot/minidump/exception_snapshot_minidump
    snapshot/minidump/memory_snapshot_minidump
    snapshot/minidump/minidump_annotation_reader
    snapshot/minidump/minidump_context_converter
    snapshot/minidump/minidump_simple_string_dictionary_reader
    snapshot/minidump/minidump_string_list_reader
    snapshot/minidump/minidump_string_reader
    snapshot/minidump/module_snapshot_minidump
    snapshot/minidump/process_snapshot_minidump
    snapshot/minidump/system_snapshot_minidump
    snapshot/minidump/thread_snapshot_minidump
    snapshot/win/capture_memory_delegate_win
    snapshot/win/cpu_context_win
    snapshot/win/exception_snapshot_win
    snapshot/win/memory_map_region_snapshot_win
    snapshot/win/module_snapshot_win
    snapshot/win/pe_image_annotations_reader
    snapshot/win/pe_image_reader
    snapshot/win/pe_image_resource_reader
    snapshot/win/process_reader_win
    snapshot/win/process_snapshot_win
    snapshot/win/process_subrange_reader
    snapshot/win/system_snapshot_win
    snapshot/win/thread_snapshot_win
)

target_compile_definitions(
    crashpad_snapshot
    PRIVATE
    -DNOMINMAX
    -DUNICODE
    -DWIN32
    -DWIN32_LEAN_AND_MEAN
)

target_link_libraries(
    crashpad_snapshot
    PRIVATE
    mini_chromium
    crashpad_compat

    powrprof
)

add_executable(
    crashpad_handler
    WIN32
    handler/crash_report_upload_thread
    handler/handler_main
    handler/main
    handler/minidump_to_upload_parameters
    handler/prune_crash_reports_thread
    handler/user_stream_data_source
    handler/win/crash_report_exception_handler
)

target_compile_definitions(
    crashpad_handler
    PRIVATE
    -DNOMINMAX
    -DUNICODE
    -DWIN32
    -DWIN32_LEAN_AND_MEAN
)

target_include_directories(
    crashpad_handler
    PRIVATE
    compat/win
    vendor/mini_chromium
)

target_link_libraries(
    crashpad_handler
    PRIVATE
    crashpad_client
    crashpad_minidump
    crashpad_snapshot
    crashpad_tools
    getopt

    # win libs
    Comctl32
)

# Make a .com file copy from the .exe with subsystem console
add_custom_command(
    TARGET crashpad_handler POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:crashpad_handler>
        "$<TARGET_FILE_DIR:crashpad_handler>/crashpad_handler.com"
)

add_custom_command(
    TARGET crashpad_handler POST_BUILD
    COMMAND editbin
        -nologo
        -subsystem:console
        "$<TARGET_FILE_DIR:crashpad_handler>/crashpad_handler.com"
)

set_source_files_properties(
    util/misc/capture_context_win.asm
    PROPERTIES
    COMPILE_FLAGS "/safeseh"
)

set_source_files_properties(
    util/win/safe_terminate_process.asm
    PROPERTIES
    COMPILE_FLAGS "/safeseh"
)
