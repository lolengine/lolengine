//
// Lol Engine
//
// Copyright: (c) 2010-2013 Benjamin Litzelmann
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#if !defined __LOL_INPUT_DEVICE_H__
#define __LOL_INPUT_DEVICE_H__

#if defined LOL_INPUT_V2

#include "core.h"

namespace lol
{

class InputDevice
{
public:
    /** Get the name of this input device */
    const String& GetName();

    /** Get the index of the corresponding key, needed to call GetKey */
    int GetKeyIndex(const char* name) const { return GetItemIndex(name, m_keynames); }
    /** Get the index of the corresponding axis, needed to call GetAxis */
    int GetAxisIndex(const char* name) const { return GetItemIndex(name, m_axisnames); }
    /** Get the index of the corresponding cursor, needed to call GetCursor */
    int GetCursorIndex(const char* name) const { return GetItemIndex(name, m_cursornames); }

    /** Get the current state of the given key, true being pressed and false being released */
    bool GetKey(int index) const { return m_keys[index]; }
    /** Get the current value of the given axis. Devices should cap this value between -1 and 1 as much as possible, through it is not guaranteed */
    float GetAxis(int index) const { return m_axis[index].m1 * m_axis[index].m2; }
    /** Get the current value of the given cursor, 0,0 being the bottom-left corner and 1,1 being the top-right corner */
    vec2 GetCursor(int index) const { return m_cursors[index].m1; }
    /** Get the coordinate of the pixel the cursor is currently over, 0,0 being the bottom-left corner. */
    ivec2 GetCursorPixel(int index) const { return m_cursors[index].m2; }


    /** Set a per-device-axis sensitivity factor. The value returned by the operating system will be multiplied by this value before being returned by GetAxis */
    void SetAxisSensitivity(int index, float sensitivity) { m_axis[index].m2 = sensitivity; }
    /** Get the per-device-axis sensitivity factor. The value returned by the operating system will be multiplied by this value before being returned by GetAxis */
    float GetAxisSensitivity(int index) const { return m_axis[index].m2; }

    /** Get a list of the name of all available keys in this device */
    const Array<String>& GetAllKeys() const { return m_keynames; }
    /** Get a list of the name of all available axis in this device */
    const Array<String>& GetAllAxis() const { return m_axisnames; }
    /** Get a list of the name of all available cursors in this device */
    const Array<String>& GetAllCursors() const { return m_cursornames; }

    /** Get an input device by its name */
    static InputDevice* Get(const char* name) { return GetDevice(name); }
	/** Set whether the mouse cursor should be captured. */
	static void CaptureMouse(bool activated) { m_capturemouse = activated; }

protected:
	// TODO: hide all of this in a InputDeviceData?

    String m_name;

    Array<String> m_keynames;
    Array<String> m_axisnames;
    Array<String> m_cursornames;

    /** key states (pressed/released) */
    Array<bool> m_keys;
    /** axis states (value and sensitivity) */
    Array<float, float> m_axis;
    /** cursor position */
    Array<vec2, ivec2> m_cursors;

	static bool m_capturemouse;

    InputDevice(const char* name) : m_name(name)
    {
        devices.Push(this);
    }

    ~InputDevice()
    {
        for (int i = 0; i < devices.Count(); ++i)
        {
            if (devices[i] == this)
            {
                devices.Remove(i);
                return;
            }
        }
    }

private:
    static Array<InputDevice*> devices;

    template <typename T>
    int GetItemIndex(const char* name, const Array<String, T>& Array) const
    {
        for (int i = 0; i < Array.Count(); ++i)
        {
            if (Array[i] == name)
                return i;
        }
        return -1;
    }

    static InputDevice* GetDevice(const char* name)
    {
        for (int i = 0; i < devices.Count(); ++i)
        {
            if (devices[i]->m_name == name)
                return devices[i];
        }
        return nullptr;
    }
};

} /* namespace lol */

#endif // LOL_INPUT_V2

#endif // __LOL_INPUT_DEVICE_H__

