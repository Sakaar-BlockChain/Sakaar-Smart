#include "sm_semantic.h"

#define ScopeType_None      0b000
#define ScopeType_Class     0b001
#define ScopeType_Function  0b010
#define ScopeType_Loop      0b100

#define SM_Error_Func       (-1)
#define SM_Error_Loop       (-2)
#define SM_Error_Class      (-3)
#define SM_Error_ClassFunc  (-4)

int scan_node(struct object_st *obj, short scope_type) {
    struct ast_node *node = obj->data;
    short new_scope_type = scope_type;
    if (node->main_type == MainType_Expr)
    if (node->main_type == MainType_Stmt) {
        switch (node->type) {
            case StmtType_Params:
            case StmtType_Return:
                if ((scope_type & ScopeType_Function) != ScopeType_Function) return SM_Error_Func;
                break;
            case StmtType_Break:
            case StmtType_Continue:
                if ((scope_type & ScopeType_Loop) != ScopeType_Loop) return SM_Error_Loop;
                break;
            case StmtType_Extends:
                if ((scope_type & ScopeType_Class) != ScopeType_Class) return SM_Error_Class;
                break;
            case StmtType_PUB_Func:
            case StmtType_STC_Func:
            case StmtType_PRI_Func:
                if ((scope_type & ScopeType_Class) != ScopeType_Class) return SM_Error_ClassFunc;
            case StmtType_Func:
                new_scope_type = ScopeType_Function;
                break;
            case StmtType_Class:
                new_scope_type |= ScopeType_Class;
                break;
            case StmtType_While:
            case StmtType_DoWhile:
                new_scope_type |= ScopeType_Loop;
                break;
        }
    }
    int result = 0;
    for (size_t i = 0; i < node->next->size && result == 0; i++)
        result = scan_node(node->next->data[i], new_scope_type);
    return result;
}