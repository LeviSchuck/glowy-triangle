#pragma once

// From Earth Engine's post on 
// http://stackoverflow.com/questions/9692630/implementing-haskells-maybe-monad-in-c11

template<typename T>
class Maybe{
private:
    Maybe(T t){
        get = [t](){ return t; };
    }
    Maybe(){}
    std::function<T ()> get;
public:
    typedef T content_type;

    template<typename WhenJust, typename WhenNothing>
    auto on(WhenJust &&whenJust, WhenNothing &&whenNothing) 
        -> decltype(whenNothing()){
        if(get==nullptr) return whenNothing();
        else return whenJust(get());
    }
    template<typename U>
    friend Maybe<U> just(U u);
    template<typename U>
    friend Maybe<U> nothing();
};

template<typename T>
Maybe<T> just(T t){
    return Maybe<T>(t);
}

template<typename T>
Maybe<T> nothing(){
    return Maybe<T>();
}

template<typename T, typename BinderFunction>
auto operator >>(Maybe<T> m, BinderFunction bind) 
    -> Maybe<typename decltype(bind(*((T*)nullptr)))::content_type> {
    return m.on([bind](T v){
        return bind(v);
    },[](){
        return nothing<typename decltype(bind(*((T*)nullptr)))::content_type>();
    });
}