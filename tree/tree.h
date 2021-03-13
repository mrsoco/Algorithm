//
// Created by DELL on 2021/3/5.
//

#ifndef ALGORITHM_TREE_H
#define ALGORITHM_TREE_H

#include <cstdint>
#include <list>

template<typename Tp>
class Tree {
public:
    typedef Tp 	    			        value_type;
    typedef value_type*			        pointer;
    typedef const value_type*           const_pointer;
    typedef value_type&                 reference;
    typedef const value_type&           const_reference;
    typedef value_type*          		iterator;
    typedef const value_type*			const_iterator;
    typedef std::size_t                 size_type;
    typedef std::ptrdiff_t                   	    difference_type;
    typedef std::reverse_iterator<iterator>	        reverse_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

    struct Node {
        Node(const_reference value,  Node* child = nullptr, Node* brother = nullptr)
            : value_(new value_type(value))
            , child_(child)
            , brother_(brother) {
        }

        Node(const_pointer value, Node* child = nullptr, Node* brother = nullptr)
            : value_(value)
            , child_(child)
            , brother_(brother) {
        }

        const_pointer value() const { return value_; }
        void set_value(const_pointer value) { value_ = value; }

        const Node* child() const { return child_; }
        void set_child(Node* child) { child_ = child; }

        const Node* brother() const { return brother_; }
        void set_brother(Node* brother) { brother_ = brother; }

        /** 结点的度，即子结点树 */
        size_type Degree() const;

        /** 当前结点的孩子结点的最大度 */
       // uint32_t MaxDegree() const;

        size_type Level() const;

    private:
        pointer             value_;

        /** first child */
        Node*               child_;

        /** first brother of from left to right */
        Node*               brother_;

        friend class Tree<Tp>;
    };

public:
    static Node* CreateNode(const_reference value, Node* child = nullptr, Node* brother = nullptr);
    static Node* CreateNode(const_pointer value = nullptr, Node* child = nullptr, Node* brother = nullptr);

    explicit Tree(const Node* root = nullptr);
    ~Tree();

    Tree& RootAppendChild(Node* child);
    Node* RootFirstChild() const;
    Node* RootFirstBrother() const;


public:
    bool empty();
    const Node* root() const { return root_;}

    Node* Find(const_reference value);

    Tree& InsertChild(const Node* target, size_type pos, Node* node);

private:
    void search(Node*& result, const Node* node, const_reference value);

private:
    Node* root_;
};

template<typename Tp>
Tree<Tp>::Tree(const Tree::Node *root)
    : root_(const_cast<Tree::Node*>(root)){

}


template<typename Tp>
Tree<Tp>::~Tree() {
    delete root_;
}

template<typename Tp>
Tree<Tp>& Tree<Tp>::InsertChild(const Tree::Node *target, Tree::size_type pos, Tree::Node *node) {
    /** 先根遍历和后根遍历*/
    if (root_ == target) {
        if (root_->child_ == nullptr) {
            root_->set_child(node);
            return *this;
        }

        Node *last = root_->child_;
        Node* p = last->brother_;

        if (p == nullptr) {
            last->set_brother(node);
            return *this;
        }

        for (size_type i = 1; p != nullptr; p = p->brother_) {
            if (i < pos) {
                i++;
                last = p;
                continue;
            } else {
                last->set_brother(node);
                p->set_brother(node);
                node->set_brother(p->brother_);
                break;
            }
        }
    }

    return *this;
}

template<typename Tp>
typename Tree<Tp>::Node* Tree<Tp>::CreateNode(const_reference value, Tree::Node *child, Tree::Node *brother) {
    return new Node(value, child, brother);
}

template<typename Tp>
typename Tree<Tp>::Node* Tree<Tp>::CreateNode(Tree::const_pointer value,  Tree::Node *child, Tree::Node *brother) {
    return new Node(value, child, brother);
}

template<typename Tp>
Tree<Tp>& Tree<Tp>::RootAppendChild(Tree::Node *child) {
    if (root_ == nullptr) {
        return *this;
    }

    if (root_->child_ == nullptr) {
        root_->set_child(child);
        return *this;
    }

    Node* prev = root_->child_;
    Node* brother = prev;

    while (brother != nullptr) {
        prev = brother;
        brother = brother->brother_;
    }

    prev->set_brother(child);
    return *this;
}

template<typename Tp>
typename Tree<Tp>::Node *Tree<Tp>::RootFirstChild() const {
    if (root_ == nullptr) {
        return nullptr;
    }

    return root_->child();
}

template<typename Tp>
typename Tree<Tp>::Node *Tree<Tp>::RootFirstBrother() const {
    if (root_ == nullptr) {
        return nullptr;
    }

    return root_->child()->brother();
}

template<typename Tp>
typename Tree<Tp>::Node *Tree<Tp>::Find(const_reference value) {
    Node* result = nullptr;

    if (search(result, root_, value)) {
        return result;
    }

    return nullptr;
}

template<typename Tp>
void Tree<Tp>::search(Tree::Node *&result, const Tree::Node *node, const_reference value) {
    if (node == nullptr) {
        return;
    }

    if (node->value() == value) {
        result = node;
        return;
    }

    search(result, node->child(), value);
    if (result != nullptr) {
        return;
    }

    search(result, node->brother(), value);
}


#endif //ALGORITHM_TREE_H
