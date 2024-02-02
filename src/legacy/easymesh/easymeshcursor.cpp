//
//  EasyMesh-Cursor — Cursor operations
//
//  Copyright © 2012—2020 Sam Hocevar <sam@hocevar.net>
//            © 2009—2015 Cédric Lecacheur <jordx@free.fr>
//            © 2009—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
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
        if (!BD()->LoopStack().count() || std::get<0>(BD()->LoopStack().last()) != BD()->Cmdi())
            BD()->LoopStack().push(BD()->Cmdi(), loopnb);
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
        if (BD()->LoopStack().count())
        {
            std::get<1>(BD()->LoopStack().last())--;
            if (std::get<1>(BD()->LoopStack().last()) > 0)
                BD()->Cmdi() = std::get<0>(BD()->LoopStack().last()) - 1;
            else
                BD()->LoopStack().pop();
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

    m_cursors.push(m_vert.count(), m_indices.count());
}

//-----------------------------------------------------------------------------
void EasyMesh::CloseBrace()
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::CloseBrace);
        return;
    }

    m_cursors.pop();
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
void EasyMesh::SetCurColor(vec4 const &col)
{
    SetCurColorA(col);
    SetCurColorB(col);
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurColorA(vec4 const &col)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::SetColorA);
        BD()->CmdStack() << col;
        return;
    }

    BD()->ColorA() = col;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetCurColorB(vec4 const &col)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::SetColorB);
        BD()->CmdStack() << col;
        return;
    }

    BD()->ColorB() = col;
}

//-----------------------------------------------------------------------------
void EasyMesh::SetVertColor(vec4 const &col)
{
    if (BD()->IsEnabled(MeshBuildOperation::CommandRecording))
    {
        BD()->CmdStack().AddCmd(EasyMeshCmdType::SetVertColor);
        BD()->CmdStack() << col;
        return;
    }

    for (int i = std::get<0>(m_cursors.last()); i < m_vert.count(); i++)
        m_vert[i].m_color = col;
}

} /* namespace lol */
