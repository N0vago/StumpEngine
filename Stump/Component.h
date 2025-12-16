#ifndef ST_COMPONENT_H
#define ST_COMPONENT_H
class Object;
class Component
{
public:
    virtual ~Component() = default;

    Object* GetOwner() const { return owner; }

    
    virtual void OnAttach() {}   
    virtual void Awake() {}
    virtual void Start() {}      
    virtual void Update(float p_deltaTime) {}
	virtual void Destroy() {}
    virtual void Render() {}

    bool IsEnabled() const { return enabled; }
    void SetEnabled(bool p_isEnabled) { enabled = p_isEnabled; }

private:
    friend class Object;
    void SetOwner(Object* p_owner) { owner = p_owner; }

    Object* owner = nullptr;
    bool enabled = true;
};
#endif // ST_COMPONENT_H

