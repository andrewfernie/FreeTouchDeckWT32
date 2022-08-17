#include "SaveConfig.h"

int saveConfigGeneral(AsyncWebServerRequest *request)
{
    int status = 0;

    // --- Saving general config
    MSG_INFOLN("[INFO] Saving General Config");

    FILESYSTEM.remove("/config/general.json");
    File file = FILESYSTEM.open("/config/general.json", "w");
    if (!file) {
        MSG_WARNLN("[WARNING]: Failed to create /config/general.json file");
        status = 1;
        return status;
    }

    DynamicJsonDocument doc(400);

    JsonObject general = doc.to<JsonObject>();

    AsyncWebParameter *menubuttoncolor = request->getParam("menubuttoncolor", true);
    general["menubuttoncolor"] = menubuttoncolor->value().c_str();
    AsyncWebParameter *functionbuttoncolor = request->getParam("functionbuttoncolor", true);
    general["functionbuttoncolor"] = functionbuttoncolor->value().c_str();
    AsyncWebParameter *latchcolor = request->getParam("latchcolor", true);
    general["latchcolor"] = latchcolor->value().c_str();
    AsyncWebParameter *background = request->getParam("background", true);
    general["background"] = background->value().c_str();

    AsyncWebParameter *sleepenable = request->getParam("sleepenable", true);
    String sleepEnable = sleepenable->value().c_str();

    if (sleepEnable == "true") {
        general["sleepenable"] = true;
    }
    else {
        general["sleepenable"] = false;
    }

    AsyncWebParameter *beep = request->getParam("beep", true);
    String Beep = beep->value().c_str();

    if (Beep == "true") {
        general["beep"] = true;
    }
    else {
        general["beep"] = false;
    }

    // Sleep timer
    AsyncWebParameter *sleeptimer = request->getParam("sleeptimer", true);

    String sleepTimer = sleeptimer->value().c_str();
    general["sleeptimer"] = sleepTimer.toInt();

    // Modifiers

    AsyncWebParameter *modifier1 = request->getParam("modifier1", true);
    String Modifier1 = modifier1->value().c_str();
    general["modifier1"] = Modifier1.toInt();

    AsyncWebParameter *modifier2 = request->getParam("modifier2", true);
    String Modifier2 = modifier2->value().c_str();
    general["modifier2"] = Modifier2.toInt();

    AsyncWebParameter *modifier3 = request->getParam("modifier3", true);
    String Modifier3 = modifier3->value().c_str();
    general["modifier3"] = Modifier3.toInt();

    AsyncWebParameter *helperdelay = request->getParam("helperdelay", true);
    String Helperdelay = helperdelay->value().c_str();
    general["helperdelay"] = Helperdelay.toInt();

    if (serializeJsonPretty(doc, file) == 0) {
        MSG_WARNLN("[WARNING]: Failed to write to /config/general.json file");
        status = 2;
    }
    file.close();

    return status;
}

int saveCurrentConfigGeneral()
{
    int status = 0;

    // --- Saving general config
    MSG_INFOLN("[INFO] Saving General Config");

    FILESYSTEM.remove("/config/general.json");
    File file = FILESYSTEM.open("/config/general.json", "w");
    if (!file) {
        MSG_WARNLN("[WARNING]: Failed to create /config/general.json file");
        status = 1;
        return status;
    }

    DynamicJsonDocument doc(400);

    JsonObject general = doc.to<JsonObject>();

    general["menubuttoncolor"] = convertRGB565toHTML(generalconfig.menuButtonColour);
    general["functionbuttoncolor"] = convertRGB565toHTML(generalconfig.functionButtonColour);
    general["latchcolor"] = convertRGB565toHTML(generalconfig.latchedColour);
    general["background"] = convertRGB565toHTML(generalconfig.backgroundColour);

    if (generalconfig.sleepenable) {
        general["sleepenable"] = true;
    }
    else {
        general["sleepenable"] = false;
    }

    if (generalconfig.beep) {
        general["beep"] = true;
    }
    else {
        general["beep"] = false;
    }  

    general["sleeptimer"] = generalconfig.sleeptimer;

    // Modifiers

    general["modifier1"] = generalconfig.modifier1;
    general["modifier2"] = generalconfig.modifier2;
    general["modifier3"] = generalconfig.modifier3;

    general["helperdelay"] = generalconfig.helperdelay;

    if (serializeJsonPretty(doc, file) == 0) {
        MSG_WARNLN("[WARNING]: Failed to write to /config/general.json file");
        status = 2;
    }
    file.close();

    return status;
}