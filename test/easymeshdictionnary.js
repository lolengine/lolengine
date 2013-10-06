//-------------------------------------------------------------------------
//Command vars operations
//-------------------------------------------------------------------------
CmdVar("[ ]", ["Opens a new scope,\nevery operations between the braces\nwill only be done to the vertices added\nsince the start of this scope."]);
CmdVar("float", ["Fraction value", "0.0"]);
CmdVar("int", ["No decimal value", "0"]);
CmdVar("bool", ["Boolean value", "true/false", "1/0"]);
CmdVar("color", ["Red/Green/Blue/{Alpha}",
                "HEXA: #ABC{D}", "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;#AABBCC{DD}",
                "FLOAT:&nbsp;f f f f", "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(f f f f)", "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;(f)"]);
CmdVar("vec3", ["X/Y/Z as float", "&nbsp;f f f", "(f f f)", "(f)"]);

//-------------------------------------------------------------------------
//Mesh CSG operations
//-------------------------------------------------------------------------
CmdType(["csgu",  "csgunion"], "Performs a Union operation as :\n(current_scope_outside + new_scope_Outside)", [CmdArg("[ ]", "New scope")]);
CmdType(["csgs",  "csgsubstract"], "Performs a Substract operation as :\n(current_scope_Outside + new_scope_Inside-inverted)", [CmdArg("[ ]", "New scope")]);
CmdType(["csgsl", "csgsubstractloss"], "Performs a Substract operation without keeping the new_scope part", [CmdArg("[ ]", "New scope")]);
CmdType(["csga",  "csgand"], "Performs an And operation as :\n(current_scope_Inside + new_scope__Inside)", [CmdArg("[ ]", "New scope")]);
CmdType(["csgx",  "csgxor"], "Performs a Xor operation as :\n(current_scope_Outside + current_scope_Inside-inverted + new_scope_Outside + new_scope_Inside-inverted)", [CmdArg("[ ]", "New scope")]);

//-------------------------------------------------------------------------
//Mesh Base operations
//-------------------------------------------------------------------------
CmdType(["tsw", "scalewinding"], "When activated, on negative-scaling,\nnormal-vector correction will not occur");
CmdType(["sc",  "setcolor"], "Set A color", [CmdArg("color", "color")]);
CmdType(["scb", "setcolorb"], "Set B color", [CmdArg("color", "color")]);
CmdType(["scv", "setcolorv"], "Set the color of all vertices in this scope", [CmdArg("color", "color")]);
CmdType(["lp",  "loop"], "Performs a loop with the code in the braces.\nDoesn't open a new scope.", [CmdArg("int", "loopnb"), CmdArg("[ ]", "Loop command")]);

//-------------------------------------------------------------------------
//Mesh transform operations
//-------------------------------------------------------------------------
CmdType(["tx",   "translatex"], "Translate vertices along the X axis", [CmdArg("float", "n")]);
CmdType(["ty",   "translatey"], "Translate vertices along the Y axis", [CmdArg("float", "n")]);
CmdType(["tz",   "translatez"], "Translate vertices along the Z axis", [CmdArg("float", "n")]);
CmdType(["t",    "translate"], "Translate vertices", [CmdArg("vec3", "v")]);
CmdType(["rx",   "rotatex"], "Rotate vertices along the X axis", [CmdArg("float", "degree")]);
CmdType(["ry",   "rotatey"], "Rotate vertices along the Y axis", [CmdArg("float", "degree")]);
CmdType(["rz",   "rotatez"], "Rotate vertices along the Z axis", [CmdArg("float", "degree")]);
CmdType(["r",    "rotate"], "Rotate vertices along the given axis", [CmdArg("float", "degree"), CmdArg("vec3", "axis")]);
CmdType(["rj",   "radialjitter"], "Randomly move vertices along Origin-to-vertex as f(vtx) = vtx + o2v * (1.0 + rand(r))", [CmdArg("float", "r")]);
CmdType(["tax",  "taperx"], "multiply axis y/z by f(x)\nf(z) = z * (1.0 + (nz * f(x) + xoff))\nf(x) = absolute ? abs_x : x", [CmdArg("float", "ny"), CmdArg("float", "nz"), CmdArg("float", "xoff", "0.0"), CmdArg("bool", "absolute", "true")]);
CmdType(["tay",  "tapery"], "multiply axis x/z by f(y)\nf(x) = x * (1.0 + (nx * f(y) + yoff))\nf(y) = absolute ? abs_y : y", [CmdArg("float", "nx"), CmdArg("float", "nz"), CmdArg("float", "yoff", "0.0"), CmdArg("bool", "absolute", "true")]);
CmdType(["taz",  "taperz"], "multiply axis x/y by f(z)\nf(y) = y * (1.0 + (ny * f(z) + zoff))\nf(z) = absolute ? abs_z : z", [CmdArg("float", "nx"), CmdArg("float", "ny"), CmdArg("float", "zoff", "0.0"), CmdArg("bool", "absolute", "true")]);
CmdType(["twx",  "twistx"], "Twist vertices around x axis with x as rotation value\nf(p) = (RotateX(x * t + toff) * p)", [CmdArg("float", "t"), CmdArg("float", "toff", "0.0")]);
CmdType(["twy",  "twisty"], "Twist vertices around y axis with y as rotation value\nf(p) = (RotateY(y * t + toff) * p)", [CmdArg("float", "t"), CmdArg("float", "toff", "0.0")]);
CmdType(["twz",  "twistz"], "Twist vertices around z axis with z as rotation value\nf(p) = (RotateZ(z * t + toff) * p)", [CmdArg("float", "t"), CmdArg("float", "toff", "0.0")]);
CmdType(["shx",  "shearx"], "Shear vertices using x value as shear quantity\nf(z) = z + (nz * f(x) + xoff)\nf(x) = absolute ? abs_x : x", [CmdArg("float", "ny"), CmdArg("float", "nz"), CmdArg("float", "xoff", "0.0"), CmdArg("bool", "absolute", "true")]);
CmdType(["shy",  "sheary"], "Shear vertices using y value as shear quantity\nf(x) = x + (nx * f(y) + yoff)\nf(y) = absolute ? abs_y : y", [CmdArg("float", "nx"), CmdArg("float", "nz"), CmdArg("float", "yoff", "0.0"), CmdArg("bool", "absolute", "true")]);
CmdType(["shz",  "shearz"], "Shear vertices using z value as shear quantity\nf(y) = y + (ny * f(z) + zoff)\nf(z) = absolute ? abs_z : z", [CmdArg("float", "nx"), CmdArg("float", "ny"), CmdArg("float", "zoff", "0.0"), CmdArg("bool", "absolute", "true")]);
CmdType(["stx",  "stretchx"], "Stretch vertices using x value as stretch quantity\nf(z) = z + (pow(x, nz) + xoff)", [CmdArg("float", "ny"), CmdArg("float", "nz"), CmdArg("float", "xoff", "0.0")]);
CmdType(["sty",  "stretchy"], "Stretch vertices using y value as stretch quantity\nf(x) = x + (pow(y, nx) + yoff)", [CmdArg("float", "nx"), CmdArg("float", "nz"), CmdArg("float", "yoff", "0.0")]);
CmdType(["stz",  "stretchz"], "Stretch vertices using z value as stretch quantity\nf(y) = y + (pow(z, ny) + zoff)", [CmdArg("float", "nx"), CmdArg("float", "ny"), CmdArg("float", "zoff", "0.0")]);
CmdType(["bdxy", "bendxy"], "Bend vertices using x as bend quantity along y axis\nf(p) = (RotateY(x * t + toff) * p)", [CmdArg("float", "t"), CmdArg("float", "toff", "0.0")]);
CmdType(["bdxz", "bendxz"], "Bend vertices using x as bend quantity along z axis\nf(p) = (RotateZ(x * t + toff) * p)", [CmdArg("float", "t"), CmdArg("float", "toff", "0.0")]);
CmdType(["bdyx", "bendyx"], "Bend vertices using y as bend quantity along x axis\nf(p) = (RotateX(y * t + toff) * p)", [CmdArg("float", "t"), CmdArg("float", "toff", "0.0")]);
CmdType(["bdyz", "bendyz"], "Bend vertices using y as bend quantity along z axis\nf(p) = (RotateZ(y * t + toff) * p)", [CmdArg("float", "t"), CmdArg("float", "toff", "0.0")]);
CmdType(["bdzx", "bendzx"], "Bend vertices using z as bend quantity along x axis\nf(p) = (RotateX(z * t + toff) * p)", [CmdArg("float", "t"), CmdArg("float", "toff", "0.0")]);
CmdType(["bdzy", "bendzy"], "Bend vertices using z as bend quantity along y axis\nf(p) = (RotateY(z * t + toff) * p)", [CmdArg("float", "t"), CmdArg("float", "toff", "0.0")]);
CmdType(["sx",   "scalex"], "Scale vertices", [CmdArg("vec3", "s")]);
CmdType(["sy",   "scaley"], "Scale vertices", [CmdArg("vec3", "s")]);
CmdType(["sz",   "scalez"], "Scale vertices", [CmdArg("vec3", "s")]);
CmdType(["s",    "scale"], "Uniformly Scale vertices", [CmdArg("float", "s")]);
CmdType(["dup",  "duplicate"], "Duplicate scope mesh and apply commands in the brackets.", [CmdArg("[ ]", "")]);
CmdType(["mx",   "mirrorx"], "Mirror vertices through X-plane");
CmdType(["my",   "mirrory"], "Mirror vertices through Y-plane");
CmdType(["mz",   "mirrorz"], "Mirror vertices through Z-plane");
CmdType(["ch",   "chamfer"], "DOES NOT WORK: Performs a chamfer operation", [CmdArg("float", "f")]);
CmdType(["splt", "splittriangle"], "split triangles in 4 smaller ones", [CmdArg("int", "pass")]);
CmdType(["smth", "smooth"], "Smooth the mesh by subdivising it", [CmdArg("int", "pass"), CmdArg("int", "split_per_pass"), CmdArg("int", "smooth_per_pass")]);

//-------------------------------------------------------------------------
//Mesh shape operations
//-------------------------------------------------------------------------
CmdType(["ac", "addcylinder"], "Cylinder centered on (0,0,0) with BBox:\nMin: [-.5 * max(d1, d2),-.5 * h,-.5 * max(d1, d2)]\nMax: [ .5 * max(d1, d2), .5 * h,  .5 * max(d1, d2)]",
                             [CmdArg("int", "nsides"), CmdArg("float", "h"), CmdArg("float", "d1"), CmdArg("float", "d2", "d1"),
                              CmdArg("bool", "dualsides", "false"), CmdArg("bool", "smooth", "false"), CmdArg("bool", "close", "false")]);
CmdType(["asph", "addsphere"], "Sphere centered on (0,0,0) with BBox:\nMin: [-.5 * d]\nMax: [ .5 * d]", [CmdArg("int", "ndivisions"), CmdArg("float", "d")]);
CmdType(["acap", "addcapsule"], "Capsule centered on (0,0,0) with BBox:\nMin: [-.5 * d,-(.5 * d + h),-.5 * d]\nMax: [ .5 * d, (.5 * d + h), .5 * d]", [CmdArg("int", "ndivisions"), CmdArg("float", "h"), CmdArg("float", "d")]);
CmdType(["ato", "addtorus"], "Torus centered on (0,0,0) with BBox:\nMax: [-.5 * d2]\nMax: [ .5 * d2]", [CmdArg("int", "ndivisions"), CmdArg("float", "d1"), CmdArg("float", "d2")]);
CmdType(["ab", "addbox"], "Box centered on (0,0,0) with BBox:\nMin: [-.5 * size]\nMax: [ .5 * size]", [CmdArg("vec3", "size"), CmdArg("float", "chamf", "0.0")]);
CmdType(["ascb", "addsmoothchamfbox"], "Box centered on (0,0,0) with BBox:\nMin: [-.5 * size]\nMax: [ .5 * size]", [CmdArg("vec3", "size"), CmdArg("float", "chamf")]);
CmdType(["afcb", "addflatchamfbox"], "Box centered on (0,0,0) with BBox:\nMin: [-.5 * size]\nMax: [ .5 * size]", [CmdArg("vec3", "size"), CmdArg("float", "chamf")]);
CmdType(["as", "addstar"], "Append a Star centered on (0,0,0)\nContained in a disc of max(d1, d2) diameter.",
                            [CmdArg("int", "nbranches"), CmdArg("float", "d1"), CmdArg("float", "d2"),
                             CmdArg("bool", "fade", "false"), CmdArg("bool", "fade2", "false")]);
CmdType(["aes", "addexpandedstar"], "Star centered on (0,0,0)\nContained in a disc of max(max(d1, d2), max(d1 + extrad, d2 + extrad)) diameter.\nExpanded star branches use Color2.",
                            [CmdArg("int", "nbranches"), CmdArg("float", "d1"), CmdArg("float", "d2"), CmdArg("float", "extrad", "0.0")]);
CmdType(["ad", "adddisc"], "Disc centered on (0,0,0) with d diameter.", [CmdArg("int", "nsides"), CmdArg("float", "d"), CmdArg("bool", "fade", "false")]);
CmdType(["at", "addtriangle"], "Triangle centered on (0,0,0)\nContained in a disc of d diameter.", [CmdArg("float", "d"), CmdArg("bool", "fade", "false")]);
CmdType(["aq", "addquad"], "Quad centered on (0,0,0) with BBox:\nMin: [-size * .5f, 0,-size * .5f]\nMax: [ size * .5f, 0, size * .5f]", [CmdArg("float", "size"), CmdArg("bool", "fade", "false")]);
CmdType(["acg", "addcog"], "Gear centered on (0,0,0) with BBox:\nMin: [-.5 * max(d1, d2),-.5 * h,-.5 * max(d1, d2)]\nMax: [ .5 * max(d1, d2), .5 * h, .5 * max(d1, d2)]",
                            [CmdArg("float", "h"), CmdArg("float", "d10"), CmdArg("float", "d20"), CmdArg("float", "d1"), CmdArg("float", "d2"), CmdArg("float", "d12"), CmdArg("float", "d22"),
                             CmdArg("float", "sidemul", "1.0"), CmdArg("bool", "offset", "false")]);
