#pragma once

template <typename TRes,typename... TArgs>
class Action {
private:
	TRes(*action)(TArgs...);
public:
	Action(TRes(*act)(TArgs...));
	~Action();
	TRes Execute(TArgs... args);
	TRes operator()(TArgs... args);
	Action<TRes,TArgs...>& operator=(Action<TRes, TArgs...>);
	Action<TRes, TArgs...>& operator=(TRes(*act)(TArgs...));
};

template<typename TRes, typename ...TArgs>
inline Action<TRes, TArgs...>::Action(TRes(*act)(TArgs...)) : action(act)
{
	
}

template<typename TRes, typename... TArgs>
inline Action<TRes, TArgs...>::~Action()
{
	action = nullptr;
}

template<typename TRes, typename... TArgs>
inline TRes Action<TRes, TArgs...>::Execute(TArgs... args)
{
	return action(args...);
}

template<typename TRes, typename... TArgs>
inline TRes Action<TRes, TArgs...>::operator()(TArgs... args)
{
	return action(args...);
}

template<typename TRes, typename... TArgs>
inline Action<TRes, TArgs...>& Action<TRes, TArgs...>::operator=(Action<TRes, TArgs...> a)
{
	// TODO: вставьте здесь оператор return
	this->action = a.action;
	return *this;
}

template<typename TRes, typename ...TArgs>
inline Action<TRes, TArgs...>& Action<TRes, TArgs...>::operator=(TRes(*act)(TArgs...))
{
	this->action = act;
	return *this;
}

