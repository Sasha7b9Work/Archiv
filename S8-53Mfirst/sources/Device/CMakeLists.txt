# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (Device)

set(CMAKE_UNITY_BUILD OFF)

include_directories(..
                          src
                          src/Hardware/HAL
                          src/Hardware/LwIP
                          src/Hardware/VCP
                    ../common/Hardware/FatFS
                    ../common/Hardware/FDrive
                    ../common/_VS_KEIL/ARMCLANG
                    ../common/_VS_KEIL/CMSIS
                    ../common/_VS_KEIL/FatFS
                    ../common/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../common/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/STM32Cube_F4/Drivers/CMSIS/Include
                    ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../common/STM32Cube_F4/Middlewares/Third_Party/FatFs/src
                    ../common/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/drivers
                    ../common/STM32Cube_F4/Middlewares/Third_Party/LwIP/src/include
                    ../common/STM32Cube_F4/Middlewares/Third_Party/LwIP/system
                    ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    )

add_definitions(-DSTM32F429xx -DUSE_HAL_DRIVER -DDEVICE)

if(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

    add_compile_options(/wd4103)    # alignment changed after including header, may be due to missing #pragma pack(pop)
    add_compile_options(/wd4201)    # nonstandard extension used : nameless struct/union
    add_compile_options(/wd4211)    # nonstandard extension used : redefined extern to static
    add_compile_options(/wd4310)    # cast truncates constant value
    add_compile_options(/wd4505)    # 'function': unreferenced function with internal linkage has been removed
    add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
    add_compile_options(/wd4623)    # 'derived class' : default constructor was implicitly defined as deleted because a base
                                    # class default constructor is inaccessible or deleted
    add_compile_options(/wd4626)    # 'derived class' : assignment operator was implicitly defined as deleted because a base
                                    # class assignment operator is inaccessible or deleted
    add_compile_options(/wd4668)    # 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
    add_compile_options(/wd4774)    # 'function' : format string expected in argument 3 is not a string literal
    add_compile_options(/wd4820)    # 'bytes' bytes padding added after construct 'member_name'
    add_compile_options(/wd5027)    # 'derived class' : move assignment operator was implicitly defined as deleted
    add_compile_options(/wd5039)    # 'function': pointer or reference to potentially throwing function passed to
                                    # 'extern "C"' function under -EHc. Undefined behavior may occur if this function throws
                                    # an exception
    add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    add_compile_options(/wd5204)    # 'xxx': class has virtual functions, but its trivial destructor is not virtual;
                                    # instances of objects derived from this class may not be destructed correctly
    add_compile_options(/wd5219)    # implicit conversion from 'type1' to 'type2', possible loss of data
    add_compile_options(/wd5220)    # '<unnamed-tag>::SR': a non-static data member with a volatile qualified type no longer
                                    # implies that compiler generated copy/move constructors and copy/move assignment
                                    # operators are not trivia
    add_compile_options(/wd26812)   # Prefer 'enum class' over 'enum' (Enum.3)

else()


    add_definitions(-Wno-builtin-declaration-mismatch
                    -Wno-write-strings
                    -Wno-overflow
                    -Wno-int-to-pointer-cast
    )

endif()

file(GLOB VS_KEIL_ARMCLANG ../common/vs_keil/ARMCLANG/*)
file(GLOB VS_KEIL_CMSIS    ../common/vs_keil/CMSIS/core_cm3.h)
file(GLOB VS_KEIL_FATFS    ../common/vs_keil/FatFS/*)

file(GLOB LIBS_FATFS     ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/ff_gen_drv.*
                         ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/ff.*
                         ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/diskio.*
                         ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/option/ccsbcs.*
                         ../common/STM32Cube_F4/Middlewares/Third_Party/FatFS/src/option/syscall.*)

file(GLOB LIBS_CMSIS     ../common/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include/*
                         ../common/STM32Cube_F4/Drivers/CMSIS/Include/*)

file(GLOB LIBS_HAL       ../common/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.h
                         ../common/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.c)

#file(GLOB LIBS_LWIP      ../common/STM32Cube_F4/Middlewares/Third_Party/LwIP/src/core/init.c
#                         ../common/STM32Cube_F4/Middlewares/Third_Party/LwIP/src/core/netif.c
#                         ../common/STM32Cube_F4/Middlewares/Third_Party/LwIP/src/core/timeouts.c)

file(GLOB LIBS_USBD      ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_ctlreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_ioreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/usbd_cdc.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.*)

file(GLOB LIBS_USBH      ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_core.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ctlreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ioreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_pipes.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_bot.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_scsi.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.*
                         ../common/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.*)

file(GLOB HARDWARE                      src/Hardware/*.h
                                        src/Hardware/*.cpp
                                  ../common/Hardware/Sound_.*
                                  ../common/Hardware/Timer_.*
                                  ../common/Hardware/VCP_.*)
file(GLOB HARDWARE_CMSIS                src/Hardware/CMSIS/*.*)
file(GLOB HARDWARE_FATFS                src/Hardware/FatFS/*.*)
file(GLOB HARDWARE_HAL                  src/Hardware/HAL/*.*
                                  ../common/Hardware/HAL/*.*)
file(GLOB HARDWARE_LAN                  src/Hardware/LAN/*.*)
file(GLOB HARDWARE_LWIP                 src/Hardware/LwIP/*.*)
file(GLOB HARDWARE_MEMORY         ../common/Hardware/Memory/*.*
                                        src/Hardware/Memory/*.*)
file(GLOB HARDWARE_VCP                  src/Hardware/VCP/*.*)
file(GLOB HARDWARE_FDRIVE         ../common/Hardware/FDrive/*.*
                                        src/Hardware/FDrive/*.*)
file(GLOB SRC                           src/*.h
                                        src/*.cpp
                                  ../common/defines_.h
                                  ../common/Log_.*)
file(GLOB DATA                          src/Data/*.*)
file(GLOB DATA_INPUTSTORAGE             src/Data/InputStorage/*.*)
file(GLOB DATA_OUTPUTSTORAGE            src/Data/OutputStorage/*.*)
file(GLOB DISPLAY                       src/Display/*.h
                                        src/Display/*.cpp
                                  ../common/Display/*.h
                                  ../common/Display/*.cpp)
file(GLOB DISPLAY_FONT            ../common/Display/Font/*.*)
file(GLOB DISPLAY_PAINTER         ../common/Display/Painter/*.*
                                        src/Display/Painter/*.*)
file(GLOB DISPLAY_SCREEN                src/Display/Screen/*.*)
file(GLOB FPGA                          src/FPGA/*.*)
file(GLOB MENU                          src/Menu/*.*)
file(GLOB MENU_ITEMS                    src/Menu/Items/*.*)
file(GLOB MENU_PAGES                    src/Menu/Pages/*.*)
file(GLOB MENU_PAGES_CURSORS            src/Menu/Pages/Cursors/*.*)
file(GLOB MENU_PAGES_DEBUG              src/Menu/Pages/Debug/*.*)
file(GLOB MENU_PAGES_DISPLAY            src/Menu/Pages/Display/*.*)
file(GLOB MENU_PAGES_MEASURES           src/Menu/Pages/Measures/*.*)
file(GLOB MENU_PAGES_MEMORY             src/Menu/Pages/Memory/*.*)
file(GLOB MENU_PAGES_SERVICE            src/Menu/Pages/Service/*.*)
file(GLOB MENU_PAGES_SERVICE_MATH       src/Menu/Pages/Service/Math/*.*)
file(GLOB PANEL                   ../common/Panel/*.*
                                        src/Panel/*.*)
file(GLOB SETTINGS                      src/Settings/*.*
                                  ../common/Settings/SettingsTypes_.h)
file(GLOB UTILS                         src/Utils/*.*
                                  ../common/Utils/*.*)
file(GLOB UTILS_CONTAINERS        ../common/Utils/Containers/*.*)
file(GLOB SCPI                          src/SCPI/*.*)
file(GLOB SCPI_PROCESS                  src/SCPI/Process/*.*)

file(GLOB ExcludedFiles src/Menu/Pages/PageHelp.cpp
                        ../common/Hardware/HAL/HAL_ROM_stm_.cpp)

set_source_files_properties(${LIBS_FATFS} ${LIBS_HAL} ${LIBS_LWIP} ${LIBS_USBD} ${LIBS_USBH} ${HARDWARE_CMSIS}
                            ${ExcludedFiles}
                            PROPERTIES HEADER_FILE_ONLY TRUE)

set_source_files_properties(src/Menu/Pages/Display/PageAveraging.cpp
                            src/Menu/Pages/Service/Math/PageFFT.cpp
                            src/Menu/Pages/Memory/PageLatest.cpp
                            src/Menu/Pages/Memory/PageInternal.cpp
                            src/Menu/Pages/Memory/PageSetMask.cpp
                            src/Menu/Pages/Display/PageGrid.cpp
                            src/Menu/Pages/Service/PageCursorsFFT.cpp
                            src/Menu/Pages/Memory/PageSetName.cpp
                            src/Menu/Pages/Service/PageInformation.cpp
                            PROPERTIES SKIP_UNITY_BUILD_INCLUSION ON)

source_group(libs/vs_keil/ARMCLANG   FILES ${VS_KEIL_ARMCLANG})
source_group(libs/vs_keil/CMSIS      FILES ${VS_KEIL_CMSIS})
source_group(libs/vs_keil/FatFS      FILES ${VS_KEIL_FATFS})
source_group(libs/CMSIS              FILES ${LIBS_CMSIS})
source_group(libs/FatFS              FILES ${LIBS_FATFS})
#source_group(libs/LwIP               FILES ${LIBS_LWIP})
source_group(libs/HAL                FILES ${LIBS_HAL})
source_group(libs/UBSD               FILES ${LIBS_USBD})
source_group(libs/USBH               FILES ${LIBS_USBH})
source_group(Hardware                FILES ${HARDWARE})
source_group(Hardware/CMSIS          FILES ${HARDWARE_CMSIS})
source_group(Hardware/FatFS          FILES ${HARDWARE_FATFS})
source_group(Hardware/HAL            FILES ${HARDWARE_HAL})
source_group(Hardware/LAN            FILES ${HARDWARE_LAN})
source_group(Hardware/LwIP           FILES ${HARDWARE_LWIP})
source_group(Hardware/Memory         FILES ${HARDWARE_MEMORY})
source_group(Hardware/VCP            FILES ${HARDWARE_VCP})
source_group(Hardware/FDrive         FILES ${HARDWARE_FDRIVE})
source_group(src                     FILES ${SRC})
source_group(Data                    FILES ${DATA})
source_group(Data/InputStorage       FILES ${DATA_INPUTSTORAGE})
source_group(Data/OutputStorage      FILES ${DATA_OUTPUTSTORAGE})
source_group(Display                 FILES ${DISPLAY})
source_group(Display/Font            FILES ${DISPLAY_FONT})
source_group(Display/Painter         FILES ${DISPLAY_PAINTER})
source_group(Display/Screen          FILES ${DISPLAY_SCREEN})
source_group(FPGA                    FILES ${FPGA})
source_group(Menu                    FILES ${MENU})
source_group(Menu/Items              FILES ${MENU_ITEMS})
source_group(Menu/Pages              FILES ${MENU_PAGES})
source_group(Menu/Pages/Cursors      FILES ${MENU_PAGES_CURSORS})
source_group(Menu/Pages/Debug        FILES ${MENU_PAGES_DEBUG})
source_group(Menu/Pages/Display      FILES ${MENU_PAGES_DISPLAY})
source_group(Menu/Pages/Measures     FILES ${MENU_PAGES_MEASURES})
source_group(Menu/Pages/Memory       FILES ${MENU_PAGES_MEMORY})
source_group(Menu/Pages/Service      FILES ${MENU_PAGES_SERVICE})
source_group(Menu/Pages/Service/Math FILES ${MENU_PAGES_SERVICE_MATH})
source_group(Panel                   FILES ${PANEL})
source_group(Settings                FILES ${SETTINGS})
source_group(Utils                   FILES ${UTILS})
source_group(Utils/Containers        FILES ${UTILS_CONTAINERS})
source_group(SCPI                    FILES ${SCPI})
source_group(SCPI/Process            FILES ${SCPI_PROCESS})


add_library(Device ${VS_KEIL_CMSIS} ${VS_KEIL_ARMCLANG} ${VS_KEIL_FATFS} ${LIBS_CMSIS} ${LIBS_FATFS} ${LIBS_HAL}
#                   ${LIBS_LWIP}
                   ${LIBS_USBD} ${LIBS_USBH} ${HARDWARE}
                   ${HARDWARE_CMSIS} ${HARDWARE_FATFS} ${HARDWARE_HAL} ${HARDWARE_LAN} ${HARDWARE_LWIP}
                   ${HARDWARE_MEMORY} ${HARDWARE_VCP} ${HARDWARE_FDRIVE} ${SRC} ${DISPLAY} 
                   ${DATA} ${DATA_INPUTSTORAGE} ${DATA_OUTPUTSTORAGE}
                   ${DISPLAY_FONT} ${DISPLAY_PAINTER} ${DISPLAY_SCREEN} ${FPGA}
                   ${MENU} ${MENU_ITEMS} 
                   ${MENU_PAGES} ${MENU_PAGES_CURSORS} ${MENU_PAGES_DEBUG} ${MENU_PAGES_DISPLAY}
                   ${MENU_PAGES_MEASURES} ${MENU_PAGES_MEMORY}
                   ${MENU_PAGES_SERVICE} ${MENU_PAGES_SERVICE_MATH}
                   ${PANEL} ${SETTINGS}
                   ${UTILS} ${UTILS_CONTAINERS} ${SCPI} ${SCPI_PROCESS})
