//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//            (c) 2013-2014 Benjamin "Touky" Huet <huet.benjamin@gmail.com>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

#include <cfloat> /* for FLT_MAX */

#include <lol/base/array.h>
#include <lol/debug/lines.h>
#include <lol/image/color.h>

namespace lol
{

//------ PORTAL SYSTEM --------
template <typename TE> class PortalRoom;
template <typename TE> class PortalDoor;
template <typename TE> class PortalSet;

//--
namespace Debug {
template <typename TE>
void Draw(PortalDoor<TE>& port, vec4 color)
{
    vec3 points[4]; port.GetPoints(points);

    // Draw normal
    vec3 p = port.m_center + port.m_up * port.m_size.y * .5f;
    Debug::DrawLine(p, p + port.m_normal, Color::red);
    Debug::DrawLine(p, p + port.m_up, Color::green);
    // Draw door
    for (int l = 0; l < 4; l++)
        Debug::DrawLine(points[l], points[(l + 1) % 4], color);
    Debug::DrawLine(points[0], points[2], color);
    Debug::DrawLine(points[1], points[3], color);
}
}

//PortalDoor base class
template <typename TE>
class PortalDoor
{
    friend class PortalSet<TE>;
    friend class PortalRoom<TE>;
    friend void Debug::Draw<TE>(PortalDoor<TE>& port, vec4 color);

private:
    void Init()
    {
        m_center    = vec3::zero;
        m_normal    = vec3::zero;
        m_up        = vec3::zero;
        m_size      = vec2::zero;

        m_view      = mat4::identity;
        m_proj      = mat4::identity;

        m_rooms[0]  = nullptr;
        m_rooms[1]  = nullptr;
    }

public:
    //Normal portal
    PortalDoor(vec3 center, vec3 normal, vec3 up, vec2 size)
    {
        Init();

        m_center = center;
        m_normal = normal;
        m_up     = up;
        m_size   = size;
    }
    //Camera  portal
    PortalDoor(mat4 view, mat4 proj)
    {
        Init();

        m_view = view;
        m_proj = proj;
    }
    //D.Tor
    ~PortalDoor()
    {
        ConnectRooms(nullptr, nullptr);
    }

    //Connect door to room
    void ConnectRooms(class PortalRoom<TE>* front_room, class PortalRoom<TE>* back_room)
    {
        for (int i = 0; i < 2; i++)
            if (m_rooms[i] != nullptr)
                *m_rooms[i] >> this;

        m_rooms[0] = back_room;
        m_rooms[1] = front_room;

        for (int i = 0; i < 2; i++)
            if (m_rooms[i] != nullptr)
               *m_rooms[i] << this;
    }

    //--
    void DisconnectRoom(class PortalRoom<TE>* room)
    {
        for (int i = 0; i < 2; i++)
            if (m_rooms[i] != nullptr && m_rooms[i] == room)
                m_rooms[i] = nullptr;
    }

    //--
    PortalRoom<TE>* GetRoom(bool front) { return m_rooms[(int)front]; }
    PortalRoom<TE>* GetRoom(PortalRoom<TE>* room) { return (m_rooms[0] == room) ? m_rooms[1] : m_rooms[0]; }

    //Get Four portal point
    void GetPoints(vec3 *points) const
    {
        vec3 right = cross(m_normal, m_up);
        points[0] = m_center + right * m_size.x *  .5f + m_up * m_size.y;
        points[1] = m_center + right * m_size.x *  .5f;
        points[2] = m_center + right * m_size.x * -.5f;
        points[3] = m_center + right * m_size.x * -.5f + m_up * m_size.y;
    }
    //Builds the portal view proj.
    //Returns false if portal is out of the view or points are on each others.
    bool BuildViewProj(mat4 view, mat4 proj)
    {
        mat4 cam_mx = proj * view;
        mat4 inv_proj_mx = inverse(proj);

        // First: Check normal dot
        if (lol::abs(dot(mat3(inverse(view)) * vec3(0.f, 0.f, 1.f), m_normal)) < .00001f)
            return false;

        // Second: convert to screen coordinates
        vec3 port_2d[2] = { vec3(FLT_MAX), vec3(-FLT_MAX) };
        vec3 door_points[4];
        vec4 proj_p[4];

        GetPoints(door_points);

        for (int i = 0; i < 4; i++)
        {
            //W to S calculations
            proj_p[i] = cam_mx * vec4(door_points[i], 1.f);
            proj_p[i] /= proj_p[i].w;

            //Clamp points within screen
            port_2d[0] = lol::min(proj_p[i].xyz, port_2d[0]);
            port_2d[1] = lol::max(proj_p[i].xyz, port_2d[1]);
            port_2d[0] = vec3(lol::clamp(port_2d[0].xy, vec2(-1.f), vec2(1.f)), port_2d[0].z);
            port_2d[1] = vec3(lol::clamp(port_2d[1].xy, vec2(-1.f), vec2(1.f)), port_2d[1].z);
        }

        //Quit if door not within the screen
        for (int i = 0; i < 3; i++)
            if (port_2d[0][i] == port_2d[1][i])
                return false;

        //Third: Convert back to view
        ivec2 j[4] = { ivec2(0), ivec2(0, 1), ivec2(1), ivec2(1, 0) };
        vec3 frust[2] = { vec3(FLT_MAX), vec3(-FLT_MAX) };
        for (int i = 0; i < 5; i++)
        {
            int k = i % 4;
            //world calculations
            proj_p[k] = inv_proj_mx * vec4(port_2d[j[k].x].x, port_2d[j[k].y].y, (i<4)?(port_2d[0].z):(1.f), 1.f);
            proj_p[k] /= proj_p[k].w;
            proj_p[k].z = lol::abs(proj_p[k].z);

            for (int h = 0; h < 3; h++)
            {
                if (i < 4 || h > 1)
                {
                    frust[0][h] = lol::min(frust[0][h], proj_p[k][h]);
                    frust[1][h] = lol::max(frust[1][h], proj_p[k][h]);
                }
            }
        }

        //Fourth: Create frustum
        m_proj = mat4::frustum(frust[0].x, frust[1].x, frust[0].y, frust[1].y, frust[0].z, frust[1].z);
        m_view = view;

        return true;
    }

    //View proj getter (doesn't check matrix validity)
    mat4 GetViewProj() { return m_proj * m_view; }

    //--
    bool TestCollide(const vec3& point)
    {
        return TestPointVsFrustum(point, GetViewProj());
    }

    //--
    bool TestCollide(const PortalDoor& door)
    {
        vec3 door_points[4];
        vec3 res_points[4];
        ivec3 pos_test = ivec3::zero;
        bool is_in = false;

        //Get points and test them on frustum
        door.GetPoints(door_points);
        for (int i = 0; i < 4; i++)
        {
            is_in = is_in || TestPointVsFrustum(door_points[i], GetViewProj(), &res_points[i]);

            if (is_in)
                return true;

            //Add points on test stuff
            pos_test += ivec3(lol::clamp(res_points[i], vec3(-1.1f), vec3(1.1f)));
        }

        return false;

        //Check if at least one point is not on the same side as the others
        for (int i = 0; i < 3; i++)
            if (lol::abs(pos_test[i]) == 4)
                return false;

        return true;
    }

private:
    mat4                        m_view;
    mat4                        m_proj;
    vec3                        m_center;
    vec3                        m_normal;
    vec3                        m_up;
    vec2                        m_size;
    PortalRoom<TE>*             m_rooms[2]; //0: Back, 1: Front
};

//--
template <typename TE>
class PortalRoom
{
    friend class PortalSet<TE>;
    friend class PortalDoor<TE>;

public:
    PortalRoom(TE* element=nullptr)
    {
        m_element = element;
    }
    ~PortalRoom()
    {
        for (auto door : m_doors)
            door->DisconnectRoom(this);
        m_doors.Empty();
    }

    PortalRoom& operator<<(class PortalDoor<TE>* door)
    {
        m_doors.PushUnique(door);
        return *this;
    }
    PortalRoom& operator>>(class PortalDoor<TE>* door)
    {
        m_doors.RemoveSwapItem(door);
        return *this;
    }

    int GetDoorCount()              { return m_doors.Count(); }
    PortalDoor<TE>* GetDoor(int i)  { return m_doors[i]; }

private:
    //Portals associated with this room.
    array<PortalDoor<TE>*>      m_doors;
    TE*                         m_element;
};

//--
template <typename TE>
class PortalSet
{
public:
    ~PortalSet()
    {
        for (auto door : m_doors)
            delete door;
        for (auto room : m_rooms)
            delete room;
        m_doors.Empty();
        m_rooms.Empty();
    }

    //Visible room getter
    void GetVisibleRooms(PortalDoor<TE>* see_through, PortalRoom<TE>* start_room, array<PortalRoom<TE>*>& visible_rooms)
    {
        array<PortalDoor<TE>*> ignore_doors;
        GetVisibleRooms(see_through, start_room, visible_rooms, ignore_doors);
    #if LOL_BUILD_DEBUG
        for (auto room : visible_rooms)
        {
            vec4 tmp = vec4::zero;
            for (auto port : room->m_doors)
            {
                Debug::Draw(*port, Color::cyan);
                tmp += vec4(port->m_center, 1.f);
            }
            tmp /= tmp.w;
            Debug::DrawBox(tmp.xyz - vec3(1.f), tmp.xyz + vec3(1.f), Color::yellow);
        }
        for (auto port : ignore_doors)
        {
            Debug::Draw(*port, Color::magenta);
            Debug::DrawViewProj(port->m_view, port->m_proj, Color::magenta);
        }
    #endif //LOL_BUILD_DEBUG
    }
private:
    void GetVisibleRooms(PortalDoor<TE>* see_through, PortalRoom<TE>* start_room, array<PortalRoom<TE>*>& visible_rooms, array<PortalDoor<TE>*>& ignore_doors)
    {
        for (auto door : start_room->m_doors)
        {
            if (door == see_through || ignore_doors.Find(door) != INDEX_NONE)
                continue;

            if (see_through->TestCollide(*door))
            {
                PortalRoom<TE>* other_room = door->GetRoom(start_room);
                if (visible_rooms.Find(other_room) != INDEX_NONE)
                    continue;

                ignore_doors.PushUnique(door);
                visible_rooms.PushUnique(other_room);
                door->BuildViewProj(see_through->m_view, see_through->m_proj);
                GetVisibleRooms(door, other_room, visible_rooms, ignore_doors);
            }
        }
    }
public:

    //Operator
    PortalSet<TE>& operator<<(class PortalRoom<TE>* room)
    {
        m_rooms.PushUnique(room);
        for (auto door : room->m_doors)
            m_doors.PushUnique(door);
        return *this;
    }
    //--
    PortalSet<TE>& operator>>(class PortalRoom<TE>* room)
    {
        for (auto door : room->m_doors)
            *this >> door;
        m_rooms.RemoveItem(room);
        return *this;
    }
    //--
    PortalSet<TE>& operator<<(class PortalDoor<TE>* door)
    {
        m_doors.PushUnique(door);
        return *this;
    }
    //--
    PortalSet<TE>& operator>>(class PortalDoor<TE>* door)
    {
        m_doors.RemoveItem(door);
        return *this;
    }

    //--
    int GetDoorCount()              { return m_doors.Count(); }
    PortalDoor<TE>* GetDoor(int i)  { return m_doors[i]; }
    int GetRoomCount()              { return m_rooms.Count(); }
    PortalRoom<TE>* GetRoom(int i)  { return m_rooms[i]; }

private:
    //Portals associated with this room.
    array<PortalRoom<TE>*>          m_rooms;
    array<PortalDoor<TE>*>          m_doors;
};

} /* namespace lol */

