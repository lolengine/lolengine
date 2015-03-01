//
//  Lol Engine
//
//  Copyright © 2010-2015 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(MapTest)
{
    void SetUp() {}

    void TearDown() {}

    // lolunit_declare_test(MapDeclare)
    // {
    //     map<uint8_t, uint8_t> m1;
    //     map<int, int> m2;
    //     map<float, float> m3;
    //     map<char const *, char const *> m4;
    // }

    // lolunit_declare_test(MapSet)
    // {
    //     map<int, int> m;

    //     for (int i = 0; i < 1000; i++)
    //         m[i] = -1;

    //     for (int i = 0; i < 1000; i++)
    //         m[i] = i;

    //     for (int i = 0; i < 1000; i++)
    //         lolunit_assert_equal(m[i], i);
    // }

    // lolunit_declare_test(MapHasKey)
    // {
    //     map<int, int> m;

    //     m[0] = 1;
    //     m[2] = 2;

    //     lolunit_assert(m.has_key(0));
    //     lolunit_assert(!m.has_key(1));
    //     lolunit_assert(m.has_key(2));
    // }

    // lolunit_declare_test(MapRemove)
    // {
    //     map<uint64_t, uint64_t> m;
    //     array<uint64_t> a;

    //     for (int i = 0; i < 20; i++)
    //     {
    //         a << i;
    //         m[i] = -1;
    //     }
    //     for (int i = 0; i < a.Count(); i++)
    //         m[i] = i;
    //     a.Shuffle();
    //     for (int i = 0; i < a.Count(); i++)
    //         m.remove(a[i]);
    // }

    // lolunit_declare_test(MapRemoveString)
    // {
    //     map<String, uint64_t> m;
    //     array<String> a;

    //     for (int i = 0; i < 20; i++)
    //     {
    //         a << String::Printf("test_str_%i", i);
    //         m[a.Last()] = -1;
    //     }
    //     for (int i = 0; i < a.Count(); i++)
    //         m[a[i]] = i;
    //     a.Shuffle();
    //     for (int i = 0; i < a.Count(); i++)
    //         m.remove(a[i]);
    // }

    // lolunit_declare_test(MapRemoveBug)
    // {
    //     map<uint64_t, uint64_t> m;

    //     for (int i = 0; i < 20; i++)
    //         m[i] = i;

    //     m.remove(12);
    //     m.remove(0);
    //     m.remove(17);
    //     m.remove(2);
    //     m.remove(9);
    //     m.remove(4);
    //     m.remove(15);
    //     m.remove(10);
    //     lolunit_assert_equal(m[8], 8);
    // }

    lolunit_declare_test(MapRandomAddRemove)
    {
        map<unsigned char, unsigned char> m;

        unsigned char a = 1, b = 1, c = 1;

        unsigned char presence[256];
        unsigned char value[256];

        for (int i = 0 ; i < 256 ; ++i)
        {
            presence[i] = 0;
        }

        for (int i = 0 ; i < 198 ; ++i)
        {
            // debug output
            // std::cout << "i " << i << ", a " << (int)a << ", b " << (int)b  << std::endl;

            m[a] = b;
            m.remove(b);

            presence[a] = 1;
            value[a] = b;
            presence[b] = 0;

            a = a * b + c;
            b = b * c + a;
            c = c * a + b;

            for (int j = 0 ; j < 256 ; ++j)
            {
                unsigned char v;
                if (presence[j])
                {
                    // debug output
                    // std::cout << "j " << j << " v " << (int)v << std::endl;
                    lolunit_assert(m.try_get(j, v));
                    lolunit_assert_equal(value[j], v);
                }
                else
                {
                    lolunit_assert(!m.try_get(j, v));
                }
            }
        }
    }

    // lolunit_declare_test(StringMap)
    // {
    //     map<char const *, int> m;

    //     m["foo"] = 42;
    //     m["bar"] = 12;
    //     m["baz"] = 2;

    //     int foo = m["foo"];
    //     int bar = m["bar"];
    //     int baz = m["baz"];

    //     lolunit_assert_equal(42, foo);
    //     lolunit_assert_equal(12, bar);
    //     lolunit_assert_equal(2, baz);

    //     //Big stress test
    //     array<String> bones = { "RootNode",
    //     "Cyberano_Ns:Root_$AssimpFbx$_Translation",
    //     "Cyberano_Ns:Box004_$AssimpFbx$_PreRotation",
    //     "Cyberano_Ns:Root_$AssimpFbx$_PreRotation",
    //     "Cyberano_Ns:Box004",
    //     "Cyberano_Ns:Root_$AssimpFbx$_Rotation",
    //     "Cyberano_Ns:Root",
    //     "Cyberano_Ns:Hips",
    //     "Cyberano_Ns:Spine",
    //     "Cyberano_Ns:RightUpLeg",
    //     "Cyberano_Ns:LeftUpLeg",
    //     "Cyberano_Ns:BeltSheath1",
    //     "Cyberano_Ns:RightCoat",
    //     "Cyberano_Ns:LeftCoat",
    //     "Cyberano_Ns:Spine1",
    //     "Cyberano_Ns:RightLeg",
    //     "Cyberano_Ns:RightUpLegRoll",
    //     "Cyberano_Ns:LeftUpLegRoll",
    //     "Cyberano_Ns:LeftLeg",
    //     "Cyberano_Ns:Sheath",
    //     "Cyberano_Ns:BeltSheath2",
    //     "Cyberano_Ns:BeltSheath3",
    //     "Cyberano_Ns:Spine2",
    //     "Cyberano_Ns:FrontBelt1",
    //     "Cyberano_Ns:BackBelt1",
    //     "Cyberano_Ns:RightFoot",
    //     "Cyberano_Ns:RightLegRoll",
    //     "Cyberano_Ns:LeftLegRoll",
    //     "Cyberano_Ns:LeftFoot",
    //     "Cyberano_Ns:Sword",
    //     "Cyberano_Ns:Neck",
    //     "Cyberano_Ns:RightShoulder",
    //     "Cyberano_Ns:LeftShoulder",
    //     "Cyberano_Ns:Cloth",
    //     "Cyberano_Ns:FrontBelt2",
    //     "Cyberano_Ns:RightToeBase",
    //     "Cyberano_Ns:LeftToeBase",
    //     "Cyberano_Ns:Head",
    //     "Cyberano_Ns:RightArm",
    //     "Cyberano_Ns:RightSpaulder1",
    //     "Cyberano_Ns:RightSpaulder2",
    //     "Cyberano_Ns:LeftArm",
    //     "Cyberano_Ns:LeftSpaulder1",
    //     "Cyberano_Ns:LeftCloth01",
    //     "Cyberano_Ns:MiddleCloth01",
    //     "Cyberano_Ns:RightCloth01",
    //     "Cyberano_Ns:FrontBelt3",
    //     "Cyberano_Ns:RightoeEnd",
    //     "Cyberano_Ns:LeftoeEnd",
    //     "Cyberano_Ns:HeadEnd",
    //     "Cyberano_Ns:Cap",
    //     "Cyberano_Ns:RightForeArm",
    //     "Cyberano_Ns:RightArmRoll",
    //     "Cyberano_Ns:LeftForeArm",
    //     "Cyberano_Ns:LeftArmRoll",
    //     "Cyberano_Ns:LeftCloth02",
    //     "Cyberano_Ns:MiddleCloth02",
    //     "Cyberano_Ns:RightCloth02",
    //     "Cyberano_Ns:Feather01",
    //     "Cyberano_Ns:RightHand",
    //     "Cyberano_Ns:RightForeArmRoll",
    //     "Cyberano_Ns:LeftHand",
    //     "Cyberano_Ns:LeftForeArmRoll",
    //     "Cyberano_Ns:LeftCloth03",
    //     "Cyberano_Ns:MiddleCloth03",
    //     "Cyberano_Ns:RightCloth03",
    //     "Cyberano_Ns:Feather02",
    //     "Cyberano_Ns:RightThumb1",
    //     "Cyberano_Ns:RightIndex1",
    //     "Cyberano_Ns:RightMiddle1",
    //     "Cyberano_Ns:RightRing1",
    //     "Cyberano_Ns:RightCuff",
    //     "Cyberano_Ns:LeftThumb1",
    //     "Cyberano_Ns:LeftIndex1",
    //     "Cyberano_Ns:LeftMiddle1",
    //     "Cyberano_Ns:LeftRing1",
    //     "Cyberano_Ns:LeftCloth04",
    //     "Cyberano_Ns:MiddleCloth04",
    //     "Cyberano_Ns:RightCloth04",
    //     "Cyberano_Ns:Feather03",
    //     "Cyberano_Ns:RightThumb2",
    //     "Cyberano_Ns:RightIndex2",
    //     "Cyberano_Ns:RightMiddle2",
    //     "Cyberano_Ns:RightRing2",
    //     "Cyberano_Ns:LeftThumb2",
    //     "Cyberano_Ns:LeftIndex2",
    //     "Cyberano_Ns:LeftMiddle2",
    //     "Cyberano_Ns:LeftRing2",
    //     "Cyberano_Ns:Feather04",
    //     "Cyberano_Ns:RightThumb3",
    //     "Cyberano_Ns:RightIndex3",
    //     "Cyberano_Ns:RightMiddle3",
    //     "Cyberano_Ns:RightRing3",
    //     "Cyberano_Ns:LeftThumb3",
    //     "Cyberano_Ns:LeftIndex3",
    //     "Cyberano_Ns:LeftMiddle3",
    //     "Cyberano_Ns:LeftRing3",
    //     "Cyberano_Ns:Feather05",
    //     "Cyberano_Ns:RightThumb4",
    //     "Cyberano_Ns:RightIndex4",
    //     "Cyberano_Ns:RightMiddle4",
    //     "Cyberano_Ns:RightRing4",
    //     "Cyberano_Ns:LeftThumb4",
    //     "Cyberano_Ns:LeftIndex4",
    //     "Cyberano_Ns:LeftMiddle4",
    //     "Cyberano_Ns:LeftRing4",
    //     "Cyberano_Ns:Feather06",
    //     "Cyberano_Ns:Feather07",
    //     "Cyberano_Ns:Feather08",
    //     "Cyberano_Ns:Feather09",
    //     "Cyberano_Ns:Feather10",
    //     "Cyberano_Ns:Feather11",
    //     "Cyberano_Ns:Feather12",
    //     "Cyberano_Ns:Feather13",
    //     "Cyberano_Ns:Feather14",
    //     "Cyberano_Ns:Feather15",
    //     "Cyberano_Ns:Feather16",
    //     "Cyberano_Ns:Feather17" };

    //     map<String, int> bones_map;
    //     for (int i = 0; i < bones.Count(); ++i)
    //         bones_map[bones[i]] = i;

    //     for (int i = 0; i < bones.Count(); ++i)
    //         lolunit_assert_equal(bones_map[bones[i]], i);
    // }
};

} /* namespace lol */

