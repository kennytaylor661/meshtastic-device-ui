#include "DeviceGUI.h"
#include "DisplayDriver.h"
#include "DisplayDriverConfig.h"
#include "InputDriver.h"
#if LV_USE_LIBINPUT
#include "LinuxInputDriver.h"
static LinuxInputDriver *linuxInputDriver = nullptr;
#else
#if defined(INPUTDRIVER_I2C_KBD_TYPE)
#include "I2CKeyboardInputDriver.h"
static I2CKeyboardInputDriver *keyboardDriver = nullptr;
#endif
#if defined(INPUTDRIVER_ENCODER_TYPE)
#include "EncoderInputDriver.h"
static EncoderInputDriver *encoderDriver = nullptr;
#endif
#if defined(INPUTDRIVER_MATRIX_TYPE)
#include "KeyMatrixInputDriver.h"
static KeyMatrixInputDriver *keyMatrixDriver = nullptr;
#endif
#if defined(INPUTDRIVER_BUTTON_TYPE)
#include "ButtonInputDriver.h"
static ButtonInputDriver *buttonDriver = nullptr;
#endif
#endif
#include "ILog.h"
#include "lvgl.h"
#include "ui.h"

DeviceGUI::DeviceGUI(const DisplayDriverConfig *cfg, DisplayDriver *driver) : displaydriver(driver), inputdriver(nullptr)
{
#if LV_USE_LIBINPUT
    if (cfg)
        linuxInputDriver = new LinuxInputDriver(cfg->keyboard(), cfg->pointer());
//    else
//        linuxInputDriver = InputDriver::instance();
#else
#if defined(INPUTDRIVER_I2C_KBD_TYPE)
    keyboardDriver = new I2CKeyboardInputDriver;
#endif
#if defined(INPUTDRIVER_ENCODER_TYPE)
    encoderDriver = new EncoderInputDriver;
#endif
#if defined(INPUTDRIVER_MATRIX_TYPE)
    keyMatrixDriver = new KeyMatrixInputDriver;
#endif
#if defined(INPUTDRIVER_BUTTON_TYPE)
    buttonDriver = new ButtonInputDriver;
#endif
#endif
    if (!inputdriver)
        inputdriver = InputDriver::instance();
}

void DeviceGUI::init(IClientBase *client)
{
    ILOG_DEBUG("Display driver init...");
    displaydriver->init(this);

    ILOG_DEBUG("Input driver init...");
#if LV_USE_LIBINPUT
    if (linuxInputDriver)
        linuxInputDriver->init();
#endif
#if defined(INPUTDRIVER_I2C_KBD_TYPE)
    if (keyboardDriver)
        keyboardDriver->init();
#endif
#if defined(INPUTDRIVER_ENCODER_TYPE)
    if (encoderDriver)
        encoderDriver->init();
#endif
#if defined(INPUTDRIVER_MATRIX_TYPE)
    if (keyMatrixDriver)
        keyMatrixDriver->init();
#endif
#if defined(INPUTDRIVER_BUTTON_TYPE)
    if (buttonDriver)
        buttonDriver->init();
#endif
    if (inputdriver)
        inputdriver->init();

    displaydriver->printConfig();
}

void DeviceGUI::task_handler(void)
{
    displaydriver->task_handler();
};

DeviceGUI::~DeviceGUI()
{
    delete inputdriver;
}