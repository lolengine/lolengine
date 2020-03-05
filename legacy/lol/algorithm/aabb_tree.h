//
//  Lol Engine
//
//  Copyright © 2010—2020 Sam Hocevar <sam@hocevar.net>
//            © 2013—2015 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#include <lol/debug/lines.h>
#include <lol/image/color.h>

#include <vector> // std::vector
#include <tuple>  // std::tuple

namespace lol
{

//------ AABB TREE SYSTEM -----
template <typename TE, typename TV, typename TB, size_t child_nb> class AABBTree;
template <typename TE> class Quadtree;
template <typename TE> class Octree;

//--
namespace Debug {
//--

template<typename TBB>
struct box_t
{
    TBB bbox;
    vec4 color;
};

template<typename TE>
struct elem_t
{
    TE *elem;
    int shadow;
    vec4 color;
};

template<typename TBB>
struct leaf_t
{
    int id;
    TBB bbox;
};

template<typename TREE, typename TE, typename TBB>
void DrawInner(TREE *tree,
               std::vector<box_t<TBB>> &boxes,
               std::vector<elem_t<TE>> &elements,
               std::vector<leaf_t<TBB>> &leaves, int children, vec4 color)
{
    boxes.push_back(box_t<TBB> { tree->GetAABB(), color::white });
    leaves.push_back(leaf_t<TBB> { 0, boxes.last().bbox });

    while (leaves.size() > 0)
    {
        for (auto const eid : tree->GetTree()[leaves[0].id].m_elements)
        {
            bool done = false;
            for (size_t k = 0; k < elements.size(); k++)
            {
                if (elements[k].elem == tree->GetElements()[eid].m_element)
                {
                    elements[k].shadow++;
                    done = true;
                    break;
                }
            }
            if (!done)
                elements.push(tree->GetElements()[eid].m_element, 1, color::red);
        }

        for (int i = 0; i < children; i++)
        {
            if (tree->GetTree()[leaves[0].id].m_children[i] != 0)
            {
                TBB bbox = tree->GetSubAABB(leaves[0].bbox, i);
                leaves.push(tree->GetTree()[leaves[0].id].m_children[i], bbox);
                boxes.push(bbox, color);
            }
        }
        leaves.remove(0);
    }
}

//--
template <typename TE, typename TV = void>
void Draw(Quadtree<TE>* tree, vec4 color)
{
    std::vector<box_t<box2>> boxes;
    std::vector<elem_t<TE>> elements;
    std::vector<leaf_t<box2>> leaves;

    DrawInner<Quadtree<TE>, TE, box2>(tree, boxes, elements, leaves, 4, color);

    vec3 off = vec3(0.0f, 0.1f, 0.0f);
    //vec3 add = vec3(0.0f, 0.1f, 0.0f);
    for (auto const &b : boxes)
    {
        Debug::DrawBox(vec3(b.bbox.aa.x, tree->m_debug_y_offset, b.bbox.aa.y),
                       vec3(b.bbox.bb.x, tree->m_debug_y_offset, b.bbox.bb.y),
                       b.color);
    }
    boxes.clear();

    for (auto const &e : elements)
    {
        while (e.shadow)
        {
            Debug::DrawBox(vec3(e.elem->GetAABB().aa.x, tree->m_debug_y_offset, e.elem->GetAABB().aa.y) + off * (float)e.shadow,
                           vec3(e.elem->GetAABB().bb.x, tree->m_debug_y_offset, e.elem->GetAABB().bb.y) + off * (float)e.shadow,
                           e.color);
            --e.shadow;
        }
    }
    elements.clear();
}

template <typename TE, typename TV = void>
void Draw(Octree<TE>* tree, vec4 color)
{
    std::vector<box_t<box3>> boxes;
    std::vector<elem_t<TE>> elements;
    std::vector<leaf_t<box3>> leaves;

    DrawInner<Octree<TE>, TE, box3>(tree, boxes, elements, leaves, 8, color);

    vec3 off = vec3(0.0f, 0.1f, 0.0f);
    //vec3 add = vec3(0.0f, 0.1f, 0.0f);
    for (auto const &b : boxes)
    {
        //float size = box.bb.x - box.aa.x;
        Debug::DrawBox(b.bbox.aa, /* + off * (m_size.x / size) */
                       b.bbox.bb, /* + off * (m_size.x / size) */
                       b.color);
        //off += add;
    }
    boxes.clear();

    for (auto const &e : elements)
    {
        while (e.shadow)
        {
            Debug::DrawBox(e.elem->GetAABB().aa + off * (float)e.shadow,
                           e.elem->GetAABB().bb + off * (float)e.shadow,
                           e.color);
            --e.shadow;
        }
    }
    elements.clear();
}

} // namespace Debug

//--
template <typename TE, typename TV, typename TB, size_t child_nb>
class AABBTree
{
    struct NodeLeaf
    {
        int m_parent;
        //Children pos in the list
        size_t m_children[child_nb];
        //Element list
        std::vector<int> m_elements;

        NodeLeaf(int parent)
        {
            m_parent = parent;
            for (size_t i = 0; i < child_nb; ++i)
                m_children[i] = 0;
        }
    };

    struct TreeElement
    {
        TE *m_element;
        std::vector<int> m_leaves;

        inline bool operator==(const TE*& element) { return m_element == element; }
    };

public:
    AABBTree()
    {
        m_max_depth = 1;
        m_max_element = 1;
        AddLeaf(0);
    }
    ~AABBTree()
    {
        Clear();
    }
    void CopySetup(const AABBTree<TE, TV, TB, child_nb>* src)
    {
        CopySetup(*src);
    }
    void CopySetup(const AABBTree<TE, TV, TB, child_nb>& src)
    {
        m_size = src.m_size;
        m_max_depth = src.m_max_depth;
        m_max_element = src.m_max_element;
    }

private:
    //--
    bool CleanupEmptyLeaves(int leaf=0)
    {
        int empty_children = 0;
        for (size_t i = 0; i < child_nb; ++i)
        {
            bool child_empty = false;
            if (m_tree[leaf].m_children[i] != 0)
                child_empty = CleanupEmptyLeaves(m_tree[leaf].m_children[i]);
            empty_children += (int)(m_tree[leaf].m_children[i] == 0 || child_empty);
        }
        if (empty_children == 4 && leaf != 0)
        {
            for (size_t i = 0; i < child_nb; ++i)
            {
                size_t old_leaf = m_tree[leaf].m_children[i];
                if (old_leaf != 0)
                    m_free_leaves.push_back(old_leaf);
                m_tree[leaf].m_children[i] = 0;
            }
            return true;
        }
        return false;
    }
    //--
    void RemoveElement(TE* element)
    {
        int idx = INDEX_NONE;
        for (size_t i = 0; i < m_elements.size(); ++i)
            if (m_elements[i].m_element == element)
                idx = i;

        if (idx == INDEX_NONE)
            return;

        //Remove item from tree leaves
        for (auto const &lid : m_elements[idx].m_leaves)
            m_tree[lid].m_elements.removeItem(idx);

        //Try leaves cleanup
        CleanupEmptyLeaves();
    }
    //--
    int AddElement(TE* element)
    {
        for (size_t i = 0; i < m_elements.size(); ++i)
            if (m_elements[i].m_element == element)
                return int(i);

        TreeElement new_element;
        new_element.m_element = element;
        new_element.m_leaves.clear();
        m_elements.push_back(new_element);
        return int(m_elements.size()) - 1;
    }
    //--
    int AddLeaf(int parent)
    {
        int idx(m_tree.size());
        if (m_free_leaves.size())
        {
            idx = m_free_leaves.pop_back();
            m_tree[idx] = NodeLeaf(parent);
        }
        else
            m_tree.push_back(NodeLeaf(parent));
        return idx;
    }

    //--
    bool TestLeaf(int leaf, const TB& leaf_bb, const TB& test_bb, std::vector<TE*>& elements)
    {
        bool result = false;
        if (TestAABBVsAABB(leaf_bb, test_bb))
        {
            NodeLeaf& node = m_tree[leaf];
            for (size_t i = 0; i < child_nb; ++i)
            {
                if (node.m_children[i] != 0)
                {
                    TB sub_aabb = GetSubAABB(leaf_bb, (int)i);
                    result = result | TestLeaf(node.m_children[i], sub_aabb, test_bb, elements);
                }
                else
                {
                    for (size_t j = 0; j < node.m_elements.size(); j++)
                        elements.push_unique(m_elements[node.m_elements[j]].m_element);
                    result = true;
                }
            }
        }
        return result;
    }
    //--
    bool RegisterElement(TE* element, int leaf, TB leaf_bb, int depth)
    {
        if (TestAABBVsAABB(leaf_bb, element->GetAABB()))
        {
            bool found_child = false;
            for (size_t i = 0; i < child_nb; ++i)
            {
                TB child_bb = GetSubAABB(leaf_bb, (int)i);
                int child_leaf = m_tree[leaf].m_children[i];
                //there is a child, go further down
                if (child_leaf != 0)
                    found_child = found_child | RegisterElement(element, child_leaf, child_bb, depth + 1);
            }
            if (found_child)
                return true;

            //Too many elements, we need to re-dispatch the elements
            if (int(m_tree[leaf].m_elements.size()) >= m_max_element &&
                depth < m_max_depth)
            {
                //Extract elements
                std::vector<int> elements = m_tree[leaf].m_elements;
                int new_eid = AddElement(element);
                // Add new element to list if not already here
                for (auto const eid : elements)
                    if (eid == new_eid)
                        new_eid = -1;
                if (new_eid != -1)
                    elements.push_back(new_eid);
                m_tree[leaf].m_elements.clear();
                //Add children
                for (size_t j = 0; j < child_nb; ++j)
                    m_tree[leaf].m_children[j] = AddLeaf(leaf);
                //Re-run extracted elements
                for (auto const eid : elements)
                    RegisterElement(m_elements[eid].m_element, leaf, leaf_bb, depth);
            }
            //else add to list.
            else
            {
                int idx = AddElement(element);
                m_elements[idx].m_leaves.push_unique(leaf);
                m_tree[leaf].m_elements.push_unique(idx);
            }
            return true;
        }
        return false;
    }

public:
    void                RegisterElement(TE* element)                        { RegisterElement(element, 0, GetAABB(), 0); }
    void                UnregisterElement(TE* element)                      { RemoveElement(element); }
    bool                FindElements(const TB& bbox, std::vector<TE*>& elements)  { return TestLeaf(0, GetAABB(), bbox, elements); }
    void                Clear()
    {
        m_tree.clear();
        m_elements.clear();
    }

    //--
    virtual TV          GetSubOffset(int sub) = 0;
    virtual TB          GetAABB() { return TB(-m_size * .5f, m_size * .5f); }
    virtual TB          GetSubAABB(const TB& bbox, int sub)
    {
        TV v(GetSubOffset(sub));
        TV half_vec = bbox.extent() * .5f;
        return TB(bbox.aa + half_vec * v,
                  bbox.aa + half_vec * (v + TV(1.f)));
    }

    //--
    TV                  GetSize()                       { return m_size; }
    int                 GetMaxDepth()                   { return m_max_depth; }
    int                 GetMaxElement()                 { return m_max_element; }
    void                SetSize(TV size)                { m_size = size; }
    void                SetMaxDepth(int max_depth)      { m_max_depth = max_depth; }
    void                SetMaxElement(int max_element)  { m_max_element = max_element; }

    std::vector<NodeLeaf> const & GetTree() const
    {
        return m_tree;
    }

    std::vector<TreeElement> const & GetElements() const
    {
        return m_elements;
    }

protected:
    std::vector<NodeLeaf> m_tree;        // actual tree
    std::vector<TreeElement> m_elements; // elements to leaves
    std::vector<int> m_free_leaves;      // leaves removed from tree
    TV m_size;         // Main tree size
    int m_max_depth;   // Maximum depth possible
    int m_max_element; // Maximum element per leaf
};

//--
template <typename TE>
class Quadtree : public AABBTree<TE, vec2, box2, 4>
{
    friend void Debug::Draw<TE,void>(Octree<TE>* tree, vec4 color);
public:
    Quadtree()          { m_debug_y_offset = 0.f; }
    virtual ~Quadtree() { }
    float               m_debug_y_offset;
protected:
    virtual vec2        GetSubOffset(int sub) { return vec2(ivec2(sub % 2, sub / 2)); }
};

//--
template <typename TE>
class Octree : public AABBTree<TE, vec3, box3, 8>
{
    friend void Debug::Draw<TE,void>(Octree<TE>* tree, vec4 color);
public:
    Octree()            { }
    virtual ~Octree()   { }
protected:
    virtual vec3        GetSubOffset(int sub) { return vec3(ivec3(sub % 2, sub / 4, (sub % 4) / 2)); }
};

} /* namespace lol */

