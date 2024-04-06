//前序遍历建树
static int idx = 0;
Node* build_tree(string str) {
    if (idx==str.length()){
        return nullptr;
    }
    if (str[idx] == '#') {
        idx++;
        return nullptr;
    }
    cout << "i: "<< idx << " str[i]:"<< str[idx] << endl;

    Node *node = new Node(str[idx] - '0');
    idx++;
    node->left = build_tree(str);
    node->right = build_tree(str);
    return node;    
}