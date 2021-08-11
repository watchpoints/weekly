#ifndef VISITOR_H
#define VISITOR_H

class BellTower;
class TerracottaWarriors;


class IVisitor
{
public:
	virtual ~IVisitor() {}
	virtual void Visit(BellTower *) = 0;
	virtual void Visit(TerracottaWarriors *) = 0;
};

#endif // VISITOR_H

/**

type Visitor interface {
	// Enter is called before children nodes are visited.
	// The returned node must be the same type as the input node n.
	// skipChildren returns true means children nodes should be skipped,
	// this is useful when work is done in Enter and there is no need to visit children.
	Enter(n Node) (node Node, skipChildren bool)
	// Leave is called after children nodes have been visited.
	// The returned node's type can be different from the input node if it is a ExprNode,
	// Non-expression node must be the same type as the input node n.
	// ok returns false to stop visiting.
	Leave(n Node) (node Node, ok bool)
}
**/