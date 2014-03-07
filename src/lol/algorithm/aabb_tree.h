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

#if !defined __LOL_AABB_TREE_H__
#define __LOL_AABB_TREE_H__

#include <lol/base/array.h>
#include <lol/debug/lines.h>

namespace lol
{

//------ AABB TREE SYSTEM -----
template <typename TE, typename TV, typename TB, size_t child_nb> class AABBTree;
template <typename TE> class QuadTree;
template <typename TE> class OcTree;

//--
namespace Debug {
//--

template<typename TREE, typename TE, typename TBB>
void DrawInner(TREE *tree, Array<TBB, vec4> &boxes,
               Array<TE *, int, vec4> &elements,
               Array<int, TBB> &leaves, int children, vec4 color)
{
    boxes.Push(tree->GetAABB(), vec4::one);
    leaves.Push(0, boxes.Last().m1);
    while (leaves.Count() > 0)
    {
        for (int j = 0; j < tree->GetTree()[leaves[0].m1].m_elements.Count(); j++)
        {
            bool done = false;
            for (int k = 0; k < elements.Count(); k++)
            {
                if (elements[k].m1 == tree->GetElements()[tree->GetTree()[leaves[0].m1].m_elements[j]].m_element)
                {
                    elements[k].m2++;
                    done = true;
                    break;
                }
            }
            if (!done)
                elements.Push(tree->GetElements()[tree->GetTree()[leaves[0].m1].m_elements[j]].m_element, 1, vec4::v1001);
        }

        for (int i = 0; i < children; i++)
        {
            if (tree->GetTree()[leaves[0].m1].m_children[i] != 0)
            {
                TBB bbox = tree->GetSubAABB(leaves[0].m2, i);
                leaves.Push(tree->GetTree()[leaves[0].m1].m_children[i], bbox);
                boxes.Push(bbox, color);
            }
        }
        leaves.Remove(0);
    }
}

//--
template <typename TE>
void Draw(QuadTree<TE>* tree, vec4 color)
{
    Array<box2, vec4> boxes;
    Array<TE*, int, vec4> elements;
    Array<int, box2> leaves;

    DrawInner<QuadTree<TE>, TE, box2>(tree, boxes, elements, leaves, 4, color);

    vec3 off = vec3::v010 * .1f;
    vec3 add = vec3::v010 * .1f;
    while (boxes.Count() > 0)
    {
        Debug::DrawBox(vec3(boxes[0].m1.A.x, tree->m_debug_y_offset, boxes[0].m1.A.y),
                       vec3(boxes[0].m1.B.x, tree->m_debug_y_offset, boxes[0].m1.B.y),
                       boxes[0].m2);
        boxes.Remove(0);
    }
    while (elements.Count() > 0)
    {
        while (elements[0].m2 > 0)
        {
            Debug::DrawBox(vec3(elements[0].m1->GetAABB().A.x, tree->m_debug_y_offset, elements[0].m1->GetAABB().A.y) + off * (float)elements[0].m2,
                           vec3(elements[0].m1->GetAABB().B.x, tree->m_debug_y_offset, elements[0].m1->GetAABB().B.y) + off * (float)elements[0].m2,
                           elements[0].m3);
            elements[0].m2--;
        }
        elements.Remove(0);
    }
}
//--
template <typename TE>
void Draw(OcTree<TE>* tree, vec4 color)
{
    Array<box3, vec4> boxes;
    Array<TE*, int, vec4> elements;
    Array<int, box3> leaves;

    DrawInner<OcTree<TE>, TE, box3>(tree, boxes, elements, leaves, 8, color);

    vec3 off = vec3::v010 * .1f;
    vec3 add = vec3::v010 * .1f;
    while (boxes.Count() > 0)
    {
        float size = boxes[0].m1.B.x - boxes[0].m1.A.x;
        Debug::DrawBox(vec3(boxes[0].m1.A.x, boxes[0].m1.A.y, boxes[0].m1.A.z) /* + off * (m_size.x / size) */,
                        vec3(boxes[0].m1.B.x, boxes[0].m1.B.y, boxes[0].m1.B.z) /* + off * (m_size.x / size) */,
                        boxes[0].m2);
        //off += add;
        boxes.Remove(0);
    }
    while (elements.Count() > 0)
    {
        while (elements[0].m2 > 0)
        {
            Debug::DrawBox(vec3(elements[0].m1->GetAABB().A.x, elements[0].m1->GetAABB().A.y, elements[0].m1->GetAABB().A.z) + off * (float)elements[0].m2,
                            vec3(elements[0].m1->GetAABB().B.x, elements[0].m1->GetAABB().B.y, elements[0].m1->GetAABB().B.z) + off * (float)elements[0].m2,
                            elements[0].m3);
            elements[0].m2--;
        }
        elements.Remove(0);
    }
}
//--
}

//--
template <typename TE, typename TV, typename TB, size_t child_nb>
class AABBTree
{
    struct NodeLeaf
    {
        int             m_parent;
        //Children pos in the list
        int             m_children[child_nb];
        //Element list
        Array<int>      m_elements;

        NodeLeaf(int parent)
        {
            m_parent = parent;
            for (int i = 0; i < child_nb; i++)
                m_children[i] = 0;
        }
    };

    struct TreeElement
    {
        TE*             m_element;
        Array<int>      m_leaves;

        inline bool operator==(const TE*& element) { return m_element == element; }
    };

public:
    AABBTree()
    {
        m_max_depth = 1;
        m_max_element = 1;
        AddLeaf(0);
    }
    ~AABBTree() { }

private:
    //--
    bool                CleanupEmptyLeaves(int leaf=0)
    {
        int empty_children = 0;
        for (int i = 0; i < child_nb; i++)
        {
            bool child_empty = false;
            if (m_tree[leaf].m_children[i] != 0)
                child_empty = CleanupEmptyLeaves(m_tree[leaf].m_children[i]);
            empty_children += (int)(m_tree[leaf].m_children[i] == 0 || child_empty);
        }
        if (empty_children == 4 && leaf != 0)
        {
            for (int i = 0; i < child_nb; i++)
            {
                int old_leaf = m_tree[leaf].m_children[i];
                if (old_leaf != 0)
                    m_free_leaves << old_leaf;
                m_tree[leaf].m_children[i] = 0;
            }
            return true;
        }
        return false;
    }
    //--
    void                RemoveElement(TE* element)
    {
        int idx = INDEX_NONE;
        for (int i = 0; i < m_elements.Count(); ++i)
            if (m_elements[i].m_element == element)
                idx = i;

        if (idx == INDEX_NONE)
            return;

        //Remove item from tree leaves
        for (int i = 0; i < m_elements[idx].m_leaves.Count(); i++)
            m_tree[m_elements[idx].m_leaves[i]].m_elements.RemoveItem(idx);

        //Try leaves cleanup
        CleanupEmptyLeaves();
    }
    //--
    int                 AddElement(TE* element)
    {
        for (int i = 0; i < m_elements.Count(); ++i)
            if (m_elements[i].m_element == element)
                return i;

        TreeElement new_element;
        new_element.m_element = element;
        new_element.m_leaves = Array<int>();
        m_elements << new_element;
        return m_elements.Count() - 1;
    }
    //--
    int                 AddLeaf(int parent)
    {
        int idx = m_tree.Count();
        if (m_free_leaves.Count())
        {
            idx = m_free_leaves.Pop();
            m_tree[idx] = NodeLeaf(parent);
        }
        else
            m_tree << NodeLeaf(parent);
        return idx;
    }

    //--
    bool                TestLeaf(int leaf, const TB& leaf_bb, const TB& test_bb, Array<TE*>& elements)
    {
        bool result = false;
        if (TestAABBVsAABB(leaf_bb, test_bb))
        {
            NodeLeaf& node = m_tree[leaf];
            for (int i = 0; i < child_nb; i++)
            {
                if (node.m_children[i] != 0)
                {
                    TB sub_aabb = GetSubAABB(leaf_bb, i);
                    result = result | TestLeaf(node.m_children[i], sub_aabb, test_bb, elements);
                }
                else
                {
                    for (int j = 0; j < node.m_elements.Count(); j++)
                        elements.PushUnique(m_elements[node.m_elements[j]].m_element);
                    result = true;
                }
            }
        }
        return result;
    }
    //--
    bool                RegisterElement(TE* element, int leaf, TB leaf_bb, int depth)
    {
        if (TestAABBVsAABB(leaf_bb, element->GetAABB()))
        {
            bool found_child = false;
            for (int i = 0; i < child_nb; i++)
            {
                TB child_bb = GetSubAABB(leaf_bb, i);
                int child_leaf = m_tree[leaf].m_children[i];
                //there is a child, go further down
                if (child_leaf != 0)
                    found_child = found_child | RegisterElement(element, child_leaf, child_bb, depth + 1);
            }
            if (found_child)
                return true;

            //Too much elements, we need to re-dispatch the elements
            if (m_tree[leaf].m_elements.Count() + 1 > m_max_element &&
                depth < m_max_depth)
            {
                //Extract elements
                Array<int> elements = m_tree[leaf].m_elements;
                elements.PushUnique(AddElement(element));
                m_tree[leaf].m_elements.Empty();
                //Add children
                for (int j = 0; j < child_nb; j++)
                    m_tree[leaf].m_children[j] = AddLeaf(leaf);
                //Re-run extracted elements
                while (elements.Count())
                {
                    RegisterElement(m_elements[elements[0]].m_element, leaf, leaf_bb, depth);
                    elements.Remove(0);
                }
            }
            //else add to list.
            else
            {
                int idx = AddElement(element);
                m_elements[idx].m_leaves.PushUnique(leaf);
                m_tree[leaf].m_elements.PushUnique(idx);
            }
            return true;
        }
        return false;
    }

public:
    void                RegisterElement(TE* element)                        { RegisterElement(element, 0, GetAABB(), 0); }
    void                UnregisterElement(TE* element)                      { RemoveElement(element); }
    bool                FindElements(const TB& bbox, Array<TE*>& elements)  { return TestLeaf(0, GetAABB(), bbox, elements); }
    void                Clear()
    {
        m_tree.Empty();
        m_elements.Empty();
    }

    //--
    virtual TV          GetSubOffset(int sub) = 0;
    virtual TB          GetAABB() { return TB(-m_size * .5f, m_size * .5f); }
    virtual TB          GetSubAABB(const TB& bbox, int sub)
    {
        TV v(GetSubOffset(sub));
        TV half_vec = (bbox.B - bbox.A) * .5f;
        return TB(bbox.A + half_vec * v,
                  bbox.A + half_vec * (v + TV::one));
    }

    //--
    void                SetSize(TV size)            { m_size = size; }
    void                SetMaxDepth(int max_depth)  { m_max_depth = max_depth; }

    Array<NodeLeaf> const & GetTree() const
    {
        return m_tree;
    }

    Array<TreeElement> const & GetElements() const
    {
        return m_elements;
    }

protected:
    Array<NodeLeaf>     m_tree;         //actual tree
    Array<TreeElement>  m_elements;     //elements to leaves
    Array<int>          m_free_leaves;  //leaves removed from tree
    TV                  m_size;         //Main tree size
    int                 m_max_depth;    //Maximum depth possible
    int                 m_max_element;  //Maximum element per leaf
};

//--
template <typename TE>
class QuadTree : public AABBTree<TE, vec2, box2, 4>
{
    friend void Debug::Draw<TE>(OcTree<TE>* tree, vec4 color);
public:
    QuadTree()          { m_debug_y_offset = 0.f; }
    virtual ~QuadTree() { }
    float               m_debug_y_offset;
protected:
    virtual vec2        GetSubOffset(int sub) { return vec2(ivec2(sub % 2, sub / 2)); }
};

//--
template <typename TE>
class OcTree : public AABBTree<TE, vec3, box3, 8>
{
    friend void Debug::Draw<TE>(OcTree<TE>* tree, vec4 color);
public:
    OcTree()            { }
    virtual ~OcTree()   { }
protected:
    virtual vec3        GetSubOffset(int sub) { return vec3(ivec3(sub % 2, sub / 4, (sub % 4) / 2)); }
};

} /* namespace lol */

#endif // __LOL_AABB_TREE_H__

