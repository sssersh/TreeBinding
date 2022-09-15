
struct BaseNodeHolder
{
public:
    BaseNodeHolder(const BaseNodeData* const node) :
        node(node)
    {}

    BaseNodeHolder(const BaseNodeHolder& rhs) = default;

	BaseNodeData* node;
private:
	BaseNodeHolder() = delete;
}

template<typename T>
struct NodeHolder : public BaseNodeHolder
{


}