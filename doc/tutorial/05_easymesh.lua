do_g0 = false
do_g1 = false
do_g2 = true
do_g3 = true
do_g4 = true

-- Gear 0: New style
g0 = EasyMesh.New();
if do_g0 then
    g0:BraceOpen()
        g0:SetColor("#00f")
        g0:AddBox(8, 1, 8)
        g0:TranslateY(-.25)
    g0:BraceClose()
    g0:BraceOpen()
        g0:SetColor("#f9f")
        g0:SetColorB("#f9f")
        g0:AddCog(12, 10, 5, 5, 20, 20, 5, 5, 0.1, 0)
        g0:Scale(.1)
        g0:TranslateY(-.1)
        -- csgu
    g0:BraceClose()
    g0:BraceOpen()
        g0:SetColor("#fff")
        g0:SetColorB("#000")
        g0:AddCog(12, 10, 10, 10, 20, 20, 5, 5, 0.1, 0)
        g0:Scale(.05)
        g0:TranslateX(-1.5)
        g0:TranslateY(.3)
        -- csgu
    g0:BraceClose()
    g0:BraceOpen()
        g0:SetColor("#00f")
        g0:AddBox(5, 3, 9)
        g0:TranslateX(2.5)
        -- csgs
    g0:BraceClose()
    g0:BraceOpen()
        g0:BraceOpen()
            g0:SetColor("#fff")
            g0:AddBox(3, 1.4, 2)
            g0:TranslateX(-2)
            g0:TranslateZ(-2)
            g0:BraceOpen()
                g0:SetColor("#fff")
                g0:AddBox(2.1, .7, 1.1)
                g0:TranslateY(.5)
                g0:TranslateX(-1.4)
                g0:TranslateZ(-1.4)
                -- csgs
            g0:BraceClose()
            g0:MirrorZ()
        g0:BraceClose()
        -- csgu
    g0:BraceClose()
end

-- Gear 1: Old style
g1 = EasyMesh.New();
if do_g1 then
    g1:sc("#ff9"); g1:scb("#ff9"); g1:acog(54, 10, 95, 95, 90, 90, -5, -5, 0.1, 0); g1:s(.1);
end

-- Other Gears
g2 = EasyMesh.New();
if do_g2 then
    g2:bop(); g2:sc("#0f0"); g2:ab(2, 2, 2); g2:t(.8); g2:rx(20); g2:ry(20); g2:bop(); g2:sc("#00f"); g2:ab(2); g2:tx(0); --csgu
    g2:bcl(); g2:bcl();
end

g3 = EasyMesh.New();
if do_g3 then
    g3:bop(); g3:sc("#0f0"); g3:ab(2); g3:t(.8); g3:rx(20); g3:ry(20); g3:bop(); g3:sc("#00f"); g3:ab(2); g3:tx(0); --csgs
    g3:bcl(); g3:bcl();
end

g4 = EasyMesh.New();
if do_g4 then
    g3:bop(); g3:sc("#0f0"); g3:ab(2); g3:t(.8); g3:rx(20); g3:ry(20); g3:bop(); g3:sc("#00f"); g3:ab(2); g3:tx(0); --csga
    g3:bcl(); g3:bcl();
end
