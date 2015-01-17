//-------------------------------------------------------------------------
//Command vars operations
//-------------------------------------------------------------------------
CmdVar("float", ["Fraction value", "0.0"]);
CmdVar("int", ["No decimal value", "0"]);
CmdVar("bool", ["Boolean value", "true/false", "1/0"]);
CmdVar("color", ["Red/Green/Blue/{Alpha}",
                "HEXA: #ABC{D}", "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;#AABBCC{DD}",
                "FLOAT:&nbsp;f f f f", "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(f f f f)", "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(f)"]);
CmdVar("vec3", ["X/Y/Z as float", "&nbsp;f f f", "(f f f)", "(f)"]);

//-------------------------------------------------------------------------
//scene operations
//-------------------------------------------------------------------------
CmdType(["addlight"],   "Add a light to the scene.\nUse other commands after this one to fully setup.", [CmdArg("float", "type")]);
CmdType(["position"],   "Set a position.\nWhen put after a light, sets its position.", [CmdArg("vec3", "pos")]);
CmdType(["color"],      "Set a color.", [CmdArg("color", "color")]);
CmdType(["clearcolor"], "Sets the color used for screen clearing.", [CmdArg("color", "color")]);
CmdType(["showgizmo"],  "If true, show the axis gizmo.", [CmdArg("bool", "show")]);
CmdType(["showlight"],  "If true, show the light locations.", [CmdArg("bool", "show")]);

