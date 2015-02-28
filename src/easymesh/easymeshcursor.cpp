//
// Lol Engine
//
// Copyright: (c) 2010-2015 Sam Hocevar <sam@hocevar.net>
//            (c) 2009-2015 Cédric Lecacheur <jordx@free.fr>
//            (c) 2009-2015 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

//
// The EasyMesh class
// ------------------
//

#include <lol/engine-internal.h>

namespace lol
{

//-----------------------------------------------------------------------------
void EasyMesh::LoopStart(int loopnb)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::LoopStart);
        BD()->CmdStack() << loopnb;
        return;
    }
    //Loop is only available when executing a command recording
    else if (BD()->IsEnabled(MeshBuildOperation::CommandExecution))
    {
        //Only register if we're not the current loop command
        if (!BD()->LoopStack().Count() || BD()->LoopStack().Last().m1 != BD()->Cmdi())
            BD()->LoopStack().Push(BD()->Cmdi(), loopnb);
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::LoopEnd()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::LoopEnd);
        return;
    }
    //Loop is only available when executing a command recording
    else if (BD()->IsEnabled(MeshBuildOperation::CommandExecution))
    {
        //Only register if we're not the current loop command
        if (BD()->LoopStack().Count())
        {
            BD()->LoopStack().Last().m2--;
            if (BD()->LoopStack().Last().m2 > 0)
                BD()->Cmdi() = BD()->LoopStack().Last().m1 - 1;
            else
                BD()->LoopStack().Pop();
        }
    }
}

//-----------------------------------------------------------------------------
void EasyMesh::OpenBrace()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::OpenBrace);
        return;
    }

    m_cursors.Push((int)m_vert.Count(), (int)m_indices.Count());
}

//-----------------------------------------------------------------------------
void EasyMesh::CloseBrace()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::CloseBrace);
        return;
    }

    m_cursors.Pop();
}

//-----------------------------------------------------------------------------
void EasyMesh::ToggleScaleWinding()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::ScaleWinding);
        return;
    }

    BD()->Toggle(MeshBuildOperation::ScaleWinding);
}

//-----------------------------------------------------------------------------
void EasyMesh::ToggleQuadWeighting()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::QuadWeighting);
        return;
    }

    BD()->Toggle(MeshBuildOperation::QuadWeighting);
}

//-----------------------------------------------------------------------------
void EasyMesh::TogglePostBuildNormal()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::PostBuildNormal);
        return;
    }

    BD()->Toggle(MeshBuildOperation::PostBuildComputeNormals);
}

//-----------------------------------------------------------------------------
void EasyMesh::ToggleVerticeNoCleanup()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::PreventVertCleanup);
        return;
    }

    BD()->Toggle(MeshBuildOperation::PreventVertCleanup);
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurColor(vec4 const &color)
{
    SetCurColorA(color);
    SetCurColorB(color);
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurColorA(vec4 const &color)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::SetColorA);
        BD()->CmdStack() << color;
        return;
    }

    BD()->ColorA() = color;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurColorB(vec4 const &color)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::SetColorB);
        BD()->CmdStack() << color;
        return;
    }

    BD()->ColorB() = color;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetVertColor(vec4 const &color)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::SetVertColor);
        BD()->CmdStack() << color;
        return;
    }

    for (int i = m_cursors.Last().m1; i < m_vert.Count(); i++)
        m_vert[i].m_color = color;
}

} /* namespace lol */
