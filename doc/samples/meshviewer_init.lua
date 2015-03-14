s0 = SceneSetup.New("DefaultSetup")

s0:AddLight("Point")
    s0:Position(0, 0, 15)
    s0:Color("#bbb")
s0:AddLight("Directional")
    s0:Position(0, 0, 15)
    s0:Color("#bbb")
--addlight 1 position (3 10 0) color #444

s0:Setup()
    s0:Color("#ddd")
    s0:Show("Gizmo")
    s0:Show("Light")

--SCENE SETUP END

--addlight 0.0 position (1 0 1) color #0ff
--addlight 0.0 position (-0.2 -1 -0.5) color (.0 .2 .5 1)
