#ifndef BUILTIN_HPP
#define BUILTIN_HPP

#ifdef __SS_BIND
#include <Python.h>
#endif

#include <gc/gc_allocator.h>
#include <gc/gc_cpp.h>

#include <vector>
#include <deque>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <ctype.h>
#include <limits>

#ifdef __SS_BACKTRACE
#include <exception>
#include <execinfo.h>
#include <cxxabi.h>
#endif

namespace __shedskin__ {

/* integer type */

#ifdef __SS_LONG
    typedef long long __ss_int;
#else
    typedef int __ss_int;
#endif

/* forward class declarations */

class pyobj;
class class_;
class str;
class int_;
class __ss_bool;
class float_;
class file;
class bool_;
class complex;

template <class T> class pyiter;
template <class T> class pyseq;

template <class T> class list;
template <class A, class B> class tuple2;
template <class T> class set;
template <class K, class V> class dict;

template <class T> class __iter;
template <class T> class __seqiter;
template <class T> class __setiter;
template <class T, class U> class __dictiterkeys;
template <class T, class U> class __dictitervalues;
template <class T, class U> class __dictiteritems;
class __fileiter;
class __xrange;
class __rangeiter;

class BaseException;
class Exception;
class StandardError;
class AssertionError;
class KeyError;
class ValueError;
class IndexError;
class NotImplementedError;
class IOError;
class OSError;
class SyntaxError;
class StopIteration;
class TypeError;
class RuntimeError;
class OverflowError;

/* STL types */

template<class T> class hashfunc;
template<class T> class hasheq;

#define __GC_VECTOR(T) std::vector< T, gc_allocator< T > >
#define __GC_DEQUE(T) std::deque< T, gc_allocator< T > >
#define __GC_STRING std::basic_string<char,std::char_traits<char>,gc_allocator<char> >

/* class declarations */

class pyobj : public gc {
public:
    class_ *__class__;

    virtual str *__repr__();
    virtual str *__str__();

    virtual int __hash__();
    virtual __ss_int __cmp__(pyobj *p);

    virtual __ss_bool __eq__(pyobj *p);
    virtual __ss_bool __ne__(pyobj *p);
    virtual __ss_bool __gt__(pyobj *p);
    virtual __ss_bool __lt__(pyobj *p);
    virtual __ss_bool __ge__(pyobj *p);
    virtual __ss_bool __le__(pyobj *p);

    virtual pyobj *__copy__();
    virtual pyobj *__deepcopy__(dict<void *, pyobj *> *);

    virtual __ss_int __len__();
    virtual __ss_int __int__();

    virtual __ss_bool __nonzero__();
    virtual __ss_int __index__();

    static const bool is_pyseq = false;
};

template <class T> class pyiter : public pyobj {
public:
    virtual __iter<T> *__iter__() = 0;
    virtual __ss_bool __contains__(T t);

    typedef T for_in_unit;
    typedef __iter<T> * for_in_loop;

    inline __iter<T> *for_in_init();
    inline bool for_in_has_next(__iter<T> *iter);
    inline T for_in_next(__iter<T> *iter);
};

template <class T> class pyseq : public pyiter<T> {
public:
    virtual __ss_int __len__() = 0;
    virtual T __getitem__(__ss_int i) = 0;
    virtual __ss_int __cmp__(pyobj *p);

    virtual __iter<T> *__iter__();

    typedef T for_in_unit;
    typedef size_t for_in_loop;

    inline size_t for_in_init();
    inline bool for_in_has_next(size_t i);
    inline T for_in_next(size_t &i);

    static const bool is_pyseq = true;
};

template <class R, class A> class pycall1 : public pyobj {
public:
    virtual R __call__(A a) = 0;
};
template <class R, class A, class B> class pycall2 : public pyobj {
public:
    virtual R __call__(A a, B b) = 0;
};

template <class T> class list : public pyseq<T> {
public:
    __GC_VECTOR(T) units;

    list();
    list(int count, ...);
    template <class U> list(U *iter);
    list(list<T> *p);
    list(tuple2<T, T> *p);
    list(str *s);

    void clear();
    void *__setitem__(__ss_int i, T e);
    void *__delitem__(__ss_int i);
    int empty();
    list<T> *__slice__(__ss_int x, __ss_int l, __ss_int u, __ss_int s);
    void *__setslice__(__ss_int x, __ss_int l, __ss_int u, __ss_int s, pyiter<T> *b);
    void *__setslice__(__ss_int x, __ss_int l, __ss_int u, __ss_int s, list<T> *b);
    void *__delete__(__ss_int i);
    void *__delete__(__ss_int x, __ss_int l, __ss_int u, __ss_int s);
    void *__delslice__(__ss_int a, __ss_int b);
    __ss_bool __contains__(T a);

    list<T> *__add__(list<T> *b);
    list<T> *__mul__(__ss_int b);

    template <class U> void *extend(U *iter);
    void *extend(list<T> *p);
    void *extend(tuple2<T,T> *p);
    void *extend(str *s);

    template <class U> list<T> *__iadd__(U *iter);
    list<T> *__imul__(__ss_int n);

    __ss_int index(T a);
    __ss_int index(T a, __ss_int s);
    __ss_int index(T a, __ss_int s, __ss_int e);

    __ss_int count(T a);
    str *__repr__();
    __ss_bool __eq__(pyobj *l);

    void resize(__ss_int i); /* XXX remove */

    inline T __getfast__(__ss_int i);
    inline T __getitem__(__ss_int i);
    inline __ss_int __len__();

    T pop();
    T pop(int m);
    void *remove(T e);
    void *insert(int m, T e);

    void *append(T a);

    void *reverse();
    template<class U> void *sort(__ss_int (*cmp)(T, T), U (*key)(T), __ss_int reverse);
    template<class U> void *sort(__ss_int cmp, U (*key)(T), __ss_int reverse);
    void *sort(__ss_int (*cmp)(T, T), __ss_int key, __ss_int reverse);
    void *sort(__ss_int cmp, __ss_int key, __ss_int reverse);

    list<T> *__copy__();
    list<T> *__deepcopy__(dict<void *, pyobj *> *memo);

    /* iteration */

    inline bool for_in_has_next(size_t i);
    inline T for_in_next(size_t &i);
#ifdef __SS_BIND
    list(PyObject *);
    PyObject *__to_py__();
#endif
};

template<class A, class B> class tuple2 : public pyobj {
public:
    A first;
    B second;

    tuple2();
    tuple2(int n, A a, B b);
    void __init2__(A a, B b);

    A __getfirst__();
    B __getsecond__();

    str *__repr__();
    __ss_int __len__();

    __ss_bool __eq__(tuple2<A,B> *b);
    __ss_int __cmp__(pyobj *p);
    int __hash__();

    tuple2<A,B> *__copy__();
    tuple2<A,B> *__deepcopy__(dict<void *, pyobj *> *memo);

#ifdef __SS_BIND
    tuple2(PyObject *p);
    PyObject *__to_py__();
#endif
};

template<class T> class tuple2<T,T> : public pyseq<T> {
public:
    __GC_VECTOR(T) units;

    tuple2();
    tuple2(int count, ...);
    template <class U> tuple2(U *iter);
    tuple2(list<T> *p);
    tuple2(tuple2<T, T> *p);
    tuple2(str *s);

    void __init2__(T a, T b);

    T __getfirst__();
    T __getsecond__();

    inline T __getfast__(__ss_int i);
    inline T __getitem__(__ss_int i);

    inline __ss_int __len__();

    str *__repr__();

    tuple2<T,T> *__add__(tuple2<T,T> *b);
    tuple2<T,T> *__mul__(__ss_int b);

    tuple2<T,T> *__iadd__(tuple2<T,T> *b);
    tuple2<T,T> *__imul__(__ss_int n);

    __ss_bool __contains__(T a);
    __ss_bool __eq__(pyobj *p);

    tuple2<T,T> *__slice__(__ss_int x, __ss_int l, __ss_int u, __ss_int s);

    int __hash__();

    tuple2<T,T> *__deepcopy__(dict<void *, pyobj *> *memo);
    tuple2<T,T> *__copy__();

    /* iteration */

    inline bool for_in_has_next(size_t i);
    inline T for_in_next(size_t &i);

#ifdef __SS_BIND
    tuple2(PyObject *p);
    PyObject *__to_py__();
#endif
};

class str : public pyseq<str *> {
public:
    __GC_STRING unit;
    long hash;
    bool charcache;

    str();
    str(const char *s);
    str(__GC_STRING s);
    str(const char *s, int size); /* '\0' delimiter in C */

    __ss_bool __contains__(str *s);
    str *strip(str *chars=0);
    str *lstrip(str *chars=0);
    str *rstrip(str *chars=0);
    list<str *> *split(str *sep=0, int maxsplit=-1);
    __ss_bool __eq__(pyobj *s);
    str *__add__(str *b);

    str *__join(pyseq<str *> *l, bool only_ones, int total);
    template<class U> str *join(U *);
    str *join(list<str *> *); /* XXX why can't we use pyseq<str *> *? */
    str *join(tuple2<str *, str *> *);

    str *__str__();
    str *__repr__();
    str *__mul__(__ss_int n);
    inline str *__getitem__(__ss_int n);
    inline str *__getfast__(__ss_int i);
    inline __ss_int __len__();
    str *__slice__(__ss_int x, __ss_int l, __ss_int u, __ss_int s);

    list<str *> *rsplit(str *sep = 0, int maxsplit = -1);
    tuple2<str *, str *> *rpartition(str *sep);
    tuple2<str *, str *> *partition(str *sep);
    list<str *> *splitlines(int keepends = 0);

    int __fixstart(int a, int b);
    int __checkneg(int i);

    int find(str *s, int a=0);
    int find(str *s, int a, int b);
    int rfind(str *s, int a=0);
    int rfind(str *s, int a, int b);
    int index(str *s, int a=0);
    int index(str *s, int a, int b);
    int rindex(str *s, int a=0);
    int rindex(str *s, int a, int b);

    __ss_int count(str *s, __ss_int start=0);
    __ss_int count(str *s, __ss_int start, __ss_int end);

    str *upper();
    str *lower();
    str *title();
    str *capitalize();
    str *replace(str *a, str *b, int c=-1);
    str *translate(str *table, str *delchars=0);
    str *swapcase();
    str *center(int w, str *fill=0);

    __ss_bool __ctype_function(int (*cfunc)(int));

    __ss_bool istitle();
    __ss_bool isspace();
    __ss_bool isalpha();
    __ss_bool isdigit();
    __ss_bool islower();
    __ss_bool isupper();
    __ss_bool isalnum();

    __ss_bool startswith(str *s, __ss_int start=0);
    __ss_bool startswith(str *s, __ss_int start, __ss_int end);
    __ss_bool endswith(str *s, __ss_int start=0);
    __ss_bool endswith(str *s, __ss_int start, __ss_int end);

    str *zfill(int width);
    str *expandtabs(int width=8);

    str *ljust(int width, str *fchar=0);
    str *rjust(int width, str *fchar=0);

    __ss_int __cmp__(pyobj *p);
    int __hash__();

    __ss_int __int__(); /* XXX compilation warning for int(pyseq<str *> *) */

    str *__iadd__(str *b);
    str *__imul__(__ss_int n);

    /* iteration */

    inline bool for_in_has_next(size_t i);
    inline str *for_in_next(size_t &i);

#ifdef __SS_BIND
    str(PyObject *p);
    PyObject *__to_py__();
#endif
};

void __throw_index_out_of_range();
void __throw_range_step_zero();
void __throw_set_changed();
void __throw_dict_changed();
void __throw_slice_step_zero();
void __throw_stop_iteration();

template<class K, class V> class dictentry;

const int MINSIZE = 8;

template<class K, class V> struct dict_looper {
    int pos;
    int si_used;
    dictentry<K,V> *entry;
};

template <class K, class V> class dict : public pyiter<K> {
public:
	int fill;
    int used;
    int mask;
    dictentry<K,V> *table;
    dictentry<K,V> smalltable[MINSIZE];

    dict();
    dict(int count, ...);
    template<class U> dict(U *other);
    dict(dict<K, V> *p);

    dict<K,V>& operator=(const dict<K,V>& other);

    void *__setitem__(K k, V v);
    V __getitem__(K k);
    void *__delitem__(K k);
    int do_discard(K key);
    list<K> *keys();
    list<V> *values();
    list<tuple2<K, V> *> *items();
    __ss_int __len__();
    str *__repr__();
    __ss_bool has_key(K k);
    __ss_bool __contains__(K key);
    __ss_bool __contains__(dictentry<K,V>* entry);
    void *clear();
    dict<K,V> *copy();
    V get(K k);
    V get(K k, V v);
    V pop(K k);
    tuple2<K, V> *popitem();
    template <class U> void *update(U *other);
    void *update(dict<K, V> *e);

    __ss_bool __gt__(dict<K,V> *s);
    __ss_bool __lt__(dict<K,V> *s);
    __ss_bool __ge__(dict<K,V> *s);
    __ss_bool __le__(dict<K,V> *s);
    __ss_bool __eq__(pyobj *p);

    __ss_int __cmp__(pyobj *p);
    V setdefault(K k, V v=0);

    __dictiterkeys<K, V> *__iter__() { return new __dictiterkeys<K,V>(this);}
    __dictiterkeys<K, V> *iterkeys() { return new __dictiterkeys<K,V>(this);}
    __dictitervalues<K, V> *itervalues() { return new __dictitervalues<K,V>(this);}
    __dictiteritems<K, V> *iteritems() { return new __dictiteritems<K,V>(this);}

    dict<K, V> *__deepcopy__(dict<void *, pyobj *> *memo);
    dict<K, V> *__copy__();

    void *__addtoitem__(K k, V v);

    /* iteration */

    typedef K for_in_unit;
    typedef dict_looper<K,V> for_in_loop;

    inline dict_looper<K,V> for_in_init() { dict_looper<K,V> l; l.pos = 0; l.si_used = used; return l; }
    inline bool for_in_has_next(dict_looper<K,V> &l) {
        if (l.si_used != used) {
            l.si_used = -1;
            __throw_dict_changed();
        }
        int ret = next(&l.pos, &l.entry);
        if (!ret) return false;
        return true;
    }
    inline K for_in_next(dict_looper<K,V> &l) { return l.entry->key; }

#ifdef __SS_BIND
    dict(PyObject *);
    PyObject *__to_py__();
#endif

    // used internally
    dictentry<K,V>* lookup(K key, long hash) const;
    void insert_key(K key, V value, long hash);
    void insert_clean(K key, V value, long hash);
    int next(int *pos_ptr, dictentry<K,V> **entry_ptr);
    void resize(int minused);
};

template<class T> struct setentry;

template<class T> struct set_looper {
    int pos;
    int si_used;
    setentry<T> *entry;
};

template<class T> class set : public pyiter<T> {
public:
    int frozen;
    int fill;
    int used;
    int mask;
    setentry<T> *table;
    setentry<T> smalltable[MINSIZE];
    int hash;

    template<class U> set(U *other, int frozen);
    template<class U> set(U *other);
    set(int frozen=0);

    set<T>& operator=(const set<T>& other);

    void *add(T key);
    void *add(setentry<T>* entry);
    void *discard(T key);
    int do_discard(T key);
    void *remove(T key);
    T pop();

    str* __repr__();

    __ss_bool __contains__(T key);
    __ss_bool __contains__(setentry<T>* entry);
    __ss_int __len__();

    void *clear();
    set<T> *copy();

    template <class U> void *update(U *other);
    void *update(set<T> *s);

    template <class U> set<T> *intersection(U *other);
    set<T> *intersection(set<T> *s);

    template <class U> set<T> *difference(U *other);
    set<T> *difference(set<T> *s);

    void *difference_update(set<T> *s);
    void *intersection_update(set<T> *s);
    void *symmetric_difference_update(set<T> *s);

    set<T> *__ss_union(pyiter<T> *s);
    set<T> *__ss_union(set<T> *s);
    set<T> *symmetric_difference(set<T> *s);

    set<T> *__and__(set<T> *s);
    set<T> *__or__(set<T> *s);
    set<T> *__xor__(set<T> *s);
    set<T> *__sub__(set<T> *s);

    set<T> *__iand__(set<T> *s);
    set<T> *__ior__(set<T> *s);
    set<T> *__ixor__(set<T> *s);
    set<T> *__isub__(set<T> *s);

    __ss_bool issubset(pyiter<T> *s);
    __ss_bool issubset(set<T> *s);
    __ss_bool issuperset(set<T> *s);
    __ss_bool issuperset(pyiter<T> *s);

    __ss_bool isdisjoint(set<T> *s);
    __ss_bool isdisjoint(pyiter<T> *s);

    __ss_bool __gt__(set<T> *s);
    __ss_bool __lt__(set<T> *s);
    __ss_bool __ge__(set<T> *s);
    __ss_bool __le__(set<T> *s);
    __ss_bool __eq__(pyobj *p);

    __ss_int __cmp__(pyobj *p);

    __setiter<T> *__iter__() {
        return new __setiter<T>(this);
    }

    set<T> *__copy__();
    set<T> *__deepcopy__(dict<void *, pyobj *> *memo);

    /* iteration */

    typedef T for_in_unit;
    typedef set_looper<T> for_in_loop;

    inline set_looper<T> for_in_init() { set_looper<T> l; l.pos = 0; l.si_used = used; return l; }
    inline bool for_in_has_next(set_looper<T> &l) {
        if (l.si_used != used) {
            l.si_used = -1;
            __throw_set_changed();
        }
        int ret = next(&l.pos, &l.entry);
        if (!ret) return false;
        return true;
    }
    inline T for_in_next(set_looper<T> &l) { return l.entry->key; }

#ifdef __SS_BIND
    set(PyObject *);
    PyObject *__to_py__();
#endif

    int __hash__();

    // used internally
    setentry<T>* lookup(T key, long hash) const;
    void insert_key(T key, long hash);
    void insert_clean(T key, long hash);
    int next(int *pos_ptr, setentry<T> **entry_ptr);
    void resize(int minused);
};

class __ss_bool {
public:
    int value;
    inline __ss_int operator+(__ss_bool b);
    inline __ss_bool operator==(__ss_bool b);
    inline __ss_bool operator&(__ss_bool b);
    inline __ss_bool operator|(__ss_bool b);
    inline __ss_bool operator^(__ss_bool b);
    inline bool operator!();
    inline operator bool();
    inline __ss_bool& operator=(int a);
};

class complex : public pyobj {
public:
    double real, imag;

    complex(double real=0.0, double imag=0.0);
    template<class T> complex(T t);
    complex(str *s);

    complex *__add__(complex *b);
    complex *__add__(double b);
    complex *__iadd__(complex *b);
    complex *__iadd__(double b);

    complex *__sub__(complex *b);
    complex *__sub__(double b);
    complex *__rsub__(double b);
    complex *__isub__(complex *b);
    complex *__isub__(double b);

    complex *__mul__(complex *b);
    complex *__mul__(double b);
    complex *__imul__(complex *b);
    complex *__imul__(double b);

    complex *__div__(complex *b);
    complex *__div__(double b);
    complex *__rdiv__(double b);
    complex *__idiv__(complex *b);
    complex *__idiv__(double b);

    complex *__floordiv__(complex *b);
    complex *__floordiv__(double b);
    complex *__mod__(complex *b);
    complex *__mod__(double b);
    tuple2<complex *, complex *> *__divmod__(complex *b);
    tuple2<complex *, complex *> *__divmod__(double b);

    complex *conjugate();
    complex *__pos__();
    complex *__neg__();
    double __abs__();

    str *__repr__();

    complex *parsevalue(str *s);

    __ss_bool __eq__(pyobj *p);
    int __hash__();
    __ss_bool __nonzero__();

#ifdef __SS_BIND
    complex(PyObject *);
    PyObject *__to_py__();
#endif
};

class class_: public pyobj {
public:
    int low, high;
    str *__name__;

    class_(const char *name, int low, int high);
    str *__repr__();
    __ss_bool __eq__(pyobj *c);

};

class int_ : public pyobj {
public:
    __ss_int unit;
    int_(__ss_int i);
    str *__repr__();
    __ss_bool __nonzero__();
};

class float_ : public pyobj {
public:
    double unit;
    float_(double f);
    str *__repr__();
    __ss_bool __nonzero__();
};

class bool_ : public pyobj {
public:
    __ss_bool unit;
    bool_(__ss_bool i);
    str *__repr__();
    __ss_bool __nonzero__();
    __ss_int __index__();
};

class object : public pyobj {
public:
    object();

};

template<class T> class __iter : public pyiter<T> {
public:
    T __result;
    bool __stop_iteration;

    __iter<T> *__iter__();
    virtual T next(); /* __get_next can be overloaded to avoid (slow) exception handling */
    virtual T __get_next();

    str *__repr__();
};

template <class T> class __setiter : public __iter<T> {
public:
    set<T> *p;
    int pos;
    int si_used;
    int len;
    setentry<T>* entry;

    __setiter<T>(set<T> *p);
    T next();
};

class __xrange : public pyiter<__ss_int> {
public:
    __ss_int a, b, s;

    __xrange(__ss_int a, __ss_int b, __ss_int s);
    __iter<__ss_int> *__iter__();
    __ss_int __len__();
    str *__repr__();
};

template <class T> class __seqiter : public __iter<T> {
public:
    size_t counter, size;
    pyseq<T> *p;
    __seqiter<T>();
    __seqiter<T>(pyseq<T> *p);
    T next();
};

template <class K, class V> class __dictiterkeys : public __iter<K> {
public:
    dict<K,V> *p;
    int pos;
    int si_used;
    int len;
    dictentry<K,V>* entry;

    __dictiterkeys<K, V>(dict<K, V> *p);
    K next();
};

template <class K, class V> class __dictitervalues : public __iter<V> {
public:
    dict<K,V> *p;
    int pos;
    int si_used;
    int len;
    dictentry<K,V>* entry;

    __dictitervalues<K, V>(dict<K, V> *p);
    V next();
};

template <class K, class V> class __dictiteritems : public __iter<tuple2<K, V> *> {
public:
    dict<K,V> *p;
    int pos;
    int si_used;
    int len;
    dictentry<K,V>* entry;

    __dictiteritems<K, V>(dict<K, V> *p);
    tuple2<K, V> *next();
};

static inline __ss_bool __mbool(bool c) { __ss_bool b; b.value=(int)c; return b; }

/* builtin function declarations */

template <class T> __iter<T> *___iter(pyiter<T> *p) {
    return p->__iter__();
}

file *open(str *name, str *flags = 0);
str *raw_input(str *msg = 0);

void print(int n, file *f, str *end, str *sep, ...);
void print2(file *f, int comma, int n, ...);

__ss_bool isinstance(pyobj *, class_ *);
__ss_bool isinstance(pyobj *, tuple2<class_ *, class_ *> *);

list<__ss_int> *range(__ss_int b);
list<__ss_int> *range(__ss_int a, __ss_int b, __ss_int s=1);

__xrange *xrange(__ss_int b);
__xrange *xrange(__ss_int a, __ss_int b, __ss_int s=1);

static inline double __portableround(double x) {
    if(x<0) return ceil(x-0.5);
    return floor(x+0.5);
}
inline double ___round(double a) {
    return __portableround(a);
}
inline double ___round(double a, int n) {
    return __portableround(pow((double)10,n)*a)/pow((double)10,n);
}

template<class T> inline T __abs(T t) { return t->__abs__(); }
#ifdef __SS_LONG
template<> inline __ss_int __abs(__ss_int a) { return a<0?-a:a; }
#endif
template<> inline int __abs(int a) { return a<0?-a:a; }
template<> inline double __abs(double a) { return a<0?-a:a; }
inline int __abs(__ss_bool b) { return __abs(b.value); }
double __abs(complex *c);

template<class T> str *hex(T t) {
    return t->__hex__();
}
#ifdef __SS_LONG
template<> str *hex(__ss_int a);
#endif
template<> str *hex(int a);
template<> str *hex(__ss_bool b);

template<class T> str *oct(T t) {
    return t->__oct__();
}
#ifdef __SS_LONG
template<> str *oct(__ss_int a);
#endif
template<> str *oct(int a);
template<> str *oct(__ss_bool b);

template<class T> str *bin(T t) {
    return bin(t->__index__());
}
#ifdef __SS_LONG
template<> str *bin(__ss_int a);
#endif
template<> str *bin(int a);
template<> str *bin(__ss_bool b);

str *__mod4(str *fmt, list<pyobj *> *vals);
str *__modct(str *fmt, int n, ...);
str *__modcd(str *fmt, list<str *> *l, ...);

template<class T> str *__modtuple(str *fmt, tuple2<T,T> *t);
template<class A, class B> str *__modtuple(str *fmt, tuple2<A,B> *t);

/* internal use */

#ifdef __sun
#define INFINITY __builtin_inff()
#endif
#if defined( _MSC_VER )
#define INFINITY std::numeric_limits<double>::infinity()
#endif

#define __SS_MIN(a,b) ((a) < (b) ? (a) : (b))
#define __SS_MIN3(a,b,c) (__SS_MIN((a), __SS_MIN((b), (c))))
#define __SS_MAX(a,b) ((a) > (b) ? (a) : (b))
#define __SS_MAX3(a,b,c) (__SS_MAX((a), __SS_MAX((b), (c))))

void __init();
void __start(void (*initfunc)());
void __ss_exit(int code=0);

/* slicing */

static void inline slicenr(__ss_int x, __ss_int &l, __ss_int &u, __ss_int &s, __ss_int len);

/* hashing */

static inline int hash_combine(int seed, int other) {
    return seed ^ (other + 0x9e3779b9 + (seed << 6) + (seed >> 2));
}

template<class T> inline int hasher(T t) {
    if(t == NULL) return 0;
    return t->__hash__();
}
#ifdef __SS_LONG
template<> inline int hasher(__ss_int a) { return (a==-1)?-2:a; }
#endif
template<> inline int hasher(int a) { return (a==-1)?-2:a; }
template<> inline int hasher(__ss_bool a) { return a.value; }
template<> inline int hasher(void *a) { return (intptr_t)a; }
template<> inline int hasher(double v) {
    long hipart, x; /* modified from CPython */
    int expo;
    v = frexp(v, &expo);
    v *= 2147483648.0; /* 2**31 */
    hipart = (long)v;   /* take the top 32 bits */
    v = (v - (double)hipart) * 2147483648.0; /* get the next 32 bits */
    x = hipart + (long)v + (expo << 15);
    if (x== -1)
        x = -2;
    return x;
}

template<class T> class hashfunc
{
public:
    int operator()(T t) const { return hasher<T>(t); }
};

template<class T> class hasheq {
public:
    int operator()(T t, T v) const { return __eq(t, v); }
};

/* comparison */

template<class T> inline __ss_int __cmp(T a, T b) {
    if (!a) return -1;
    return a->__cmp__(b);
}

#ifdef __SS_LONG
template<> inline __ss_int __cmp(__ss_int a, __ss_int b) {
    if(a < b) return -1;
    else if(a > b) return 1;
    return 0;
}
#endif

template<> inline __ss_int __cmp(int a, int b) {
    if(a < b) return -1;
    else if(a > b) return 1;
    return 0;
}

template<> inline __ss_int __cmp(__ss_bool a, __ss_bool b) {
    return __cmp(a.value, b.value); /* XXX */
}

template<> inline __ss_int __cmp(double a, double b) {
    if(a < b) return -1;
    else if(a > b) return 1;
    return 0;
}
template<> inline __ss_int __cmp(void *a, void *b) {
    if(a < b) return -1;
    else if(a > b) return 1;
    return 0;
}

template<class T> __ss_int cpp_cmp(T a, T b) {
    return __cmp(a, b) == -1;
}
template<class T> __ss_int cpp_cmp_rev(T a, T b) {
    return __cmp(a, b) == 1;
}
template<class T> class cpp_cmp_custom {
    typedef __ss_int (*hork)(T, T);
    hork cmp;
public:
    cpp_cmp_custom(hork a) { cmp = a; }
    __ss_int operator()(T a, T b) const { return cmp(a,b) == -1; }
};
template<class T> class cpp_cmp_custom_rev {
    typedef __ss_int (*hork)(T, T);
    hork cmp;
public:
    cpp_cmp_custom_rev(hork a) { cmp = a; }
    __ss_int operator()(T a, T b) const { return cmp(a,b) == 1; }
};
template<class T, class V> class cpp_cmp_key {
    typedef V (*hork)(T);
    hork key;
public:
    cpp_cmp_key(hork a) { key = a; }
    __ss_int operator()(T a, T b) const { return __cmp(key(a), key(b)) == -1; }
};
template<class T, class V> class cpp_cmp_key_rev {
    typedef V (*hork)(T);
    hork key;
public:
    cpp_cmp_key_rev(hork a) { key = a; }
    __ss_int operator()(T a, T b) const { return __cmp(key(a), key(b)) == 1; }
};

template<class T> struct dereference {};
template<class T> struct dereference <T*> {
    typedef T type;
};

template<class T> inline int __is_none(T *t) { return !t; }
template<class T> inline int __is_none(T) { return 0; }

/* binding */

#ifdef __SS_BIND
template<class T> T __to_ss(PyObject *p) {
    if(p==Py_None) return (T)NULL;
    return new (typename dereference<T>::type)(p); /* isn't C++ pretty :-) */
}

#ifdef __SS_LONG
template<> __ss_int __to_ss(PyObject *p);
#endif
template<> int __to_ss(PyObject *p);
template<> __ss_bool __to_ss(PyObject *p);
template<> double __to_ss(PyObject *p);
template<> void *__to_ss(PyObject *p);

template<class T> PyObject *__to_py(T t) {
    if(!t) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    return t->__to_py__();
}

#ifdef __SS_LONG
template<> PyObject *__to_py(__ss_int i);
#endif
template<> PyObject *__to_py(int i);
template<> PyObject *__to_py(__ss_bool i);
template<> PyObject *__to_py(double i);
template<> PyObject *__to_py(void *);

extern dict<void *, void *> *__ss_proxy;
#endif

/* externs */

extern class_ *cl_str_, *cl_int_, *cl_bool, *cl_float_, *cl_complex, *cl_list, *cl_tuple, *cl_dict, *cl_set, *cl_object, *cl_xrange, *cl_rangeiter;

extern __GC_VECTOR(str *) __char_cache;

extern __ss_bool True;
extern __ss_bool False;

extern list<str *> *__join_cache;

extern file *__ss_stdin, *__ss_stdout, *__ss_stderr;

/* set */

const int PERTURB_SHIFT = 5;

const int DISCARD_NOTFOUND = 0;
const int DISCARD_FOUND = 1;

const int unused = 0;
const int dummy = 1;
const int active = 2;

template<class T> struct setentry {
    long hash; // avoid rehashings...
    T key;
    int use;
};

template<class K, class V> struct dictentry {
    long hash;
    K key;
    V value;
    int use;
};

/* int */

inline __ss_int __int() { return 0; }
__ss_int __int(str *s, __ss_int base);

template<class T> inline __ss_int __int(T t) { return t->__int__(); }
#ifdef __SS_LONG
template<> inline __ss_int __int(__ss_int i) { return i; }
#endif
template<> inline __ss_int __int(int i) { return i; }
template<> inline __ss_int __int(str *s) { return __int(s, 10); }
template<> inline __ss_int __int(__ss_bool b) { return b.value; }
template<> inline __ss_int __int(double d) { return (__ss_int)d; }

/* float */

inline double __float() { return 0; }
template<class T> inline double __float(T t) { return t->__float__(); }
#ifdef __SS_LONG
template<> inline double __float(__ss_int p) { return p; }
#endif
template<> inline double __float(int p) { return p; }
template<> inline double __float(__ss_bool b) { return __float(b.value); }
template<> inline double __float(double d) { return d; }
template<> double __float(str *s);

/* str */

str *__str();
template<class T> str *__str(T t);
template<> str *__str(double t);
#ifdef __SS_LONG
str *__str(__ss_int t, __ss_int base=10);
#endif
str *__str(int t, int base=10);
str *__str(__ss_bool b);

template<class T> str *repr(T t);
template<> str *repr(double t);
#ifdef __SS_LONG
template<> str *repr(__ss_int t);
#endif
template<> str *repr(int t);
template<> str *repr(__ss_bool b);
template<> str *repr(void *t);

str *__add_strs(int n, str *a, str *b, str *c);
str *__add_strs(int n, str *a, str *b, str *c, str *d);
str *__add_strs(int n, str *a, str *b, str *c, str *d, str *e);
str *__add_strs(int n, ...);

/* exceptions */

// stacktrace.h (c) 2008, Timo Bingmann from http://idlebox.net/
// published under the WTFPL v2.0

/** Print a demangled stack backtrace of the caller function to FILE* out. */

#ifdef __SS_BACKTRACE
static void print_stacktrace(FILE *out)
{
    fprintf(out, "\nTraceback (most recent call last):\n"); 

    // storage array for stack trace address data
    void* addrlist[64];

    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0) {
        fprintf(out, "  <empty, possibly corrupt>\n");
        return;
    }

    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = addrlen-1; i > 0; i--)
    {
        char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

        // find parentheses and +address offset surrounding the mangled name:
        // ./module(function+0x15c) [0x8048a6d]
        for (char *p = symbollist[i]; *p; ++p)
        {
            if (*p == '(')
                begin_name = p;
            else if (*p == '+')
                begin_offset = p;
            else if (*p == ')' && begin_offset) {
                end_offset = p;
                break;
            }
        }

        if (begin_name && begin_offset && end_offset
            && begin_name < begin_offset)
        {
            *begin_name++ = '\0';
            *begin_offset++ = '\0';
            *end_offset = '\0';

            // mangled name is now in [begin_name, begin_offset) and caller
            // offset in [begin_offset, end_offset). now apply
            // __cxa_demangle():

            int status;
            char* ret = abi::__cxa_demangle(begin_name,
                                            funcname, &funcnamesize, &status);
            if (status == 0) {
                funcname = ret; // use possibly realloc()-ed string
                if(strncmp(funcname, "__shedskin__::", 14) != 0)
                    //fprintf(out, "  %s : %s+%s\n", symbollist[i], funcname, begin_offset);
                    fprintf(out, "  %s : %s\n", symbollist[i], funcname);
            }
            else {
                // demangling failed. Output function name as a C function with
                // no arguments.
                //fprintf(out, "  %s : %s()+%s\n",
                //        symbollist[i], begin_name, begin_offset);
            }
        }
        else
        {
            // couldn't parse the line? print the whole line.
            //fprintf(out, "  %s\n", symbollist[i]);
        }
    }

    free(funcname);
    free(symbollist);
}
#endif

extern class_ *cl_stopiteration, *cl_assertionerror, *cl_eoferror, *cl_floatingpointerror, *cl_keyerror, *cl_indexerror, *cl_typeerror, *cl_ioerror, *cl_valueerror, *cl_zerodivisionerror, *cl_keyboardinterrupt, *cl_memoryerror, *cl_nameerror, *cl_notimplementederror, *cl_oserror, *cl_overflowerror, *cl_runtimeerror, *cl_syntaxerror, *cl_systemerror, *cl_systemexit;

class BaseException : public pyobj {
public:
    str *message; 
    BaseException(str *message=0) { 
        __init__(message); 
#ifdef __SS_BACKTRACE
        print_stacktrace(stdout);
#endif
    }
    void __init__(str *message) { 
        if(message) 
            this->message = message;
        else
            this->message = new str("");
    }
    void __init__(void *) { /* XXX test 148 */
        this->message = new str("");
    }
    str *__repr__() {
        return __add_strs(4, this->__class__->__name__, new str("('"), message, new str("',)"));
    }
    str *__str__() { 
        return message;
    }
};

class Exception: public BaseException {
public:
    Exception(str *message=0) : BaseException(message) {}

#ifdef __SS_BIND
   virtual PyObject *__to_py__() { return PyExc_Exception; }
#endif
};

class StopIteration : public Exception {
public:
    StopIteration(str *message=0) : Exception(message) { this->__class__ = cl_stopiteration; }
};

class StandardError : public Exception {
public:
    StandardError(str *message=0) : Exception(message) {}
};

class AssertionError : public StandardError {
public:
    AssertionError(str *message=0) : StandardError(message) { this->__class__ = cl_assertionerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_AssertionError; }
#endif
};

class EOFError : public StandardError {
public:
    EOFError(str *message=0) : StandardError(message) { this->__class__ = cl_eoferror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_EOFError; }
#endif
};

class FloatingPointError : public StandardError {
public:
    FloatingPointError(str *message=0) : StandardError(message) { this->__class__ = cl_floatingpointerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_FloatingPointError; }
#endif
};

class KeyError : public StandardError {
public:
    KeyError(str *message=0) : StandardError(message) { this->__class__ = cl_keyerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_KeyError; }
#endif
};

class IndexError : public StandardError {
public:
    IndexError(str *message=0) : StandardError(message) { this->__class__ = cl_indexerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_IndexError; }
#endif
};

class TypeError : public StandardError {
public:
    TypeError(str *message=0) : StandardError(message) { this->__class__ = cl_typeerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_TypeError; }
#endif
};

class IOError : public StandardError {
public:
    int __ss_errno;
    str *filename;
    str *message;
    str *strerror;

    IOError(str *message=0);
    str *__str__();
    str *__repr__();

#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_IOError; }
#endif
};

class KeyboardInterrupt : public BaseException {
public:
    KeyboardInterrupt(str *message=0) : BaseException(message) { this->__class__ = cl_keyboardinterrupt; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_KeyboardInterrupt; }
#endif
};

class MemoryError : public StandardError {
public:
    MemoryError(str *message=0) : StandardError(message) { this->__class__ = cl_memoryerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_MemoryError; }
#endif
};

class NameError : public StandardError {
public:
    NameError(str *message=0) : StandardError(message) { this->__class__ = cl_nameerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_NameError; }
#endif
};

class NotImplementedError : public StandardError {
public:
    NotImplementedError(str *message=0) : StandardError(message) { this->__class__ = cl_notimplementederror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_NotImplementedError; }
#endif
};

class OSError : public StandardError {
public:
    int __ss_errno;
    str *filename;
    str *message;
    str *strerror;

    OSError(str *message=0);
    str *__str__();
    str *__repr__();

#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_OSError; }
#endif
};

class OverflowError : public StandardError {
public:
    OverflowError(str *message=0) : StandardError(message) { this->__class__ = cl_overflowerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_OverflowError; }
#endif
};

class RuntimeError : public StandardError {
public:
    RuntimeError(str *message=0) : StandardError(message) { this->__class__ = cl_runtimeerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_RuntimeError; }
#endif
};

class SyntaxError : public StandardError {
public:
    SyntaxError(str *message=0) : StandardError(message) { this->__class__ = cl_syntaxerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_SyntaxError; }
#endif
};

class SystemError : public StandardError {
public:
    SystemError(str *message=0) : StandardError(message) { this->__class__ = cl_systemerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_SystemError; }
#endif
};

class SystemExit : public BaseException {
public:
    int code;
    SystemExit(str *message) : BaseException(message) { this->__class__ = cl_systemexit; this->code = 1; }
    SystemExit(__ss_int code) { this->__class__ = cl_systemexit; this->message = __str(code); this->code = code; }
    SystemExit() { this->__class__ = cl_systemexit; this->message = __str(0); this->code = 0; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_SystemExit; }
#endif
};

class ValueError : public StandardError {
public:
    ValueError(str *message=0) : StandardError(message) { this->__class__ = cl_valueerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_ValueError; }
#endif
};

class ZeroDivisionError : public StandardError {
public:
    ZeroDivisionError(str *message=0) : StandardError(message) { this->__class__ = cl_zerodivisionerror; }
#ifdef __SS_BIND
    PyObject *__to_py__() { return PyExc_ZeroDivisionError; }
#endif
};

#ifndef __SS_NOASSERT
#define ASSERT(x, y) if(!(x)) throw new AssertionError(y);
#else
#define ASSERT(x, y)
#endif

#define FAST_FOR(i, l, u, s, t1, t2) \
    if(s==0) \
        __throw_range_step_zero(); \
    for(__ ## t1 = l, __ ## t2 = u; ; __ ## t1 += s) { \
        if (s >= 0) { if (__ ## t1 >= __ ## t2) break; } \
        else { if (__ ## t1 <= __ ## t2) break; } \
        i=__ ## t1; \

#define FOR_IN(e, iter, temp, i, t) \
    __ ## temp = iter; \
    __ ## i = -1; \
    __ ## t = __ ## temp->for_in_init(); \
    while(__ ## temp->for_in_has_next(__ ## t)) \
    { \
        __ ## i ++; \
        e = __ ## temp->for_in_next(__ ## t);

#define FOR_IN_ZIP(a,b, k,l, t,u, n,m) \
    __ ## m = __SS_MIN(k->units.size(), l->units.size()); \
    __ ## t = k; \
    __ ## u = l; \
    for(__ ## n = 0; __ ## n < __ ## m; __ ## n ++) { \
        a = (__ ## t)->units[__ ## n]; \
        b = (__ ## u)->units[__ ## n];

#define FOR_IN_ENUM(i, m, temp, n) \
    __ ## temp = m; \
    for(__ ## n = 0; (unsigned int)__ ## n < (__ ## temp)->units.size(); __ ## n ++) { \
        i = (__ ## temp)->units[__ ## n]; \

#define END_FOR }

/* typeof for MSVC */

#if defined(_MSC_VER) && _MSC_VER>=1400
namespace msvc_typeof_impl {
	/* This is a fusion of Igor Chesnokov's method (http://rsdn.ru/forum/src/1094305.aspx)
	and Steven Watanabe's method (http://lists.boost.org/Archives/boost/2006/12/115006.php)

	How it works:
	C++ allows template type inference for templated function parameters but nothing else.
	What we do is to pass the expression sent to typeof() into the templated function vartypeID()
	as its parameter, thus extracting its type. The big problem traditionally now is how to get
	that type out of the vartypeID() instance, and here's how we do it:
		1. unique_type_id() returns a monotonically increasing integer for every unique type
		   passed to it during this compilation unit. It also specialises an instance of
		   msvc_extract_type<unique_type_id, type>::id2type_impl<true>.
		2. vartypeID() returns a sized<unique_type_id> for the type where
		   sizeof(sized<unique_type_id>)==unique_type_id. We vector through sized as a means
		   of returning the unique_type_id at compile time rather than runtime.
		3. msvc_extract_type<unique_type_id> then extracts the type by using a bug in MSVC to
		   reselect the specialised child type (id2type_impl<true>) from within the specialisation
		   of itself originally performed by the above instance of unique_type_id. This bug works
		   because when MSVC calculated the signature of the specialised
		   msvc_extract_type<unique_type_id, type>::id2type_impl<true>, it does not include the
		   value of type in the signature of id2type_impl<true>. Therefore when we reselect
		   msvc_extract_type<unique_type_id>::id2type_impl<true> it erroneously returns the one
		   already in its list of instantiated types rather than correctly generating a newly
		   specialised msvc_extract_type<unique_type_id, msvc_extract_type_default_param>::id2type_impl<true>

	This bug allows the impossible and gives us a working typeof() in MSVC. Hopefully Microsoft
	won't fix this bug until they implement a native typeof.
	*/

	struct msvc_extract_type_default_param {};
	template<int ID, typename T = msvc_extract_type_default_param> struct msvc_extract_type;

	template<int ID> struct msvc_extract_type<ID, msvc_extract_type_default_param>
	{
		template<bool> struct id2type_impl;

		typedef id2type_impl<true> id2type;
	};

	template<int ID, typename T> struct msvc_extract_type : msvc_extract_type<ID, msvc_extract_type_default_param>
	{
		template<> struct id2type_impl<true> //VC8.0 specific bugfeature
		{
			typedef T type;
		};
		template<bool> struct id2type_impl;

		typedef id2type_impl<true> id2type;
	};


	template<int N> class CCounter;

	// TUnused is required to force compiler to recompile CCountOf class
	template<typename TUnused, int NTested = 0> struct CCountOf
	{
		enum
		{
			__if_exists(CCounter<NTested>) { count = CCountOf<TUnused, NTested + 1>::count }
			__if_not_exists(CCounter<NTested>) { count = NTested }
		};
	};

	template<class TTypeReg, class TUnused, int NValue> struct CProvideCounterValue { enum { value = NValue }; };

	// type_id
	#define unique_type_id(type) \
		(CProvideCounterValue< \
			/*register TYPE--ID*/ typename msvc_extract_type<CCountOf<type >::count, type>::id2type, \
			/*increment compile-time Counter*/ CCounter<CCountOf<type >::count>, \
			/*pass value of Counter*/CCountOf<type >::count \
		 >::value)

	// Lets type_id() be > than 0
	class __Increment_type_id { enum { value = unique_type_id(__Increment_type_id) }; };

	// vartypeID() returns a type with sizeof(type_id)
	template<int NSize>	class sized { char m_pad[NSize]; };
	template<typename T> typename sized<unique_type_id(T)> vartypeID(T&);
	template<typename T> typename sized<unique_type_id(const T)> vartypeID(const T&);
	template<typename T> typename sized<unique_type_id(volatile  T)> vartypeID(volatile T&);
	template<typename T> typename sized<unique_type_id(const volatile T)> vartypeID(const volatile T&);
}

#define typeof(expression) msvc_typeof_impl::msvc_extract_type<sizeof(msvc_typeof_impl::vartypeID(expression))>::id2type::type
#endif

/* file objects */

struct __file_options {
    char lastchar;
    int space;
    bool universal_mode;
    bool cr;
    __file_options() : lastchar('\n'), space(0), universal_mode(false), cr(false) {}
};

class file : public pyiter<str *> {
public:
    str *name;
    str *mode;

    FILE *f;
    __ss_int closed;
    __file_options options;
    __GC_VECTOR(char) __read_cache;

    file(FILE *g=0) : f(g) {}
    file(str *name, str *mode=0);

    virtual void * close();
    virtual void * flush();
    virtual int  __ss_fileno();
    virtual __ss_bool isatty();
    virtual str *  next();
    virtual str *  read(int n=-1);
    virtual str *  readline(int n=-1);
    list<str *> *  readlines(__ss_int size_hint=-1);
    virtual void * seek(__ss_int i, __ss_int w=0);
    virtual __ss_int tell();
    virtual void * truncate(int size);
    virtual void * write(str *s);
    template<class U> void *writelines(U *iter);
    __iter<str *> *xreadlines();
    virtual void __enter__();
    virtual void __exit__();
    virtual __iter<str *> *__iter__();
    virtual str *__repr__();
    
    virtual bool __eof();
    virtual bool __error();

    inline void __check_closed() {
        if(closed)
            throw new ValueError(new str("I/O operation on closed file"));
    }
};

class __fileiter : public __iter<str *> {
public:
    file *p;
    __fileiter(file *p);
    str *next();
};

/* with statement */

template<class T> class __With {
public:
    __With(T expr) : _expr(expr) {
        _expr->__enter__();
    }
    ~__With() {
        _expr->__exit__();
    }
    operator T() const {
        return _expr;
    }
private:
    T _expr;
};

#define WITH(e, n) {           \
    __With<typeof(e)> __with##n(e); // TODO unique id

#define WITH_VAR(e, v, n) {    \
    __With<typeof(e)> __with##n(e);      \
    typeof(e) v = __with##n;

#define END_WITH }

template<class T> static inline int __wrap(T a, int i) {
#ifndef __SS_NOWRAP
    if(i<0) return len(a)+i;
#endif
#ifndef __SS_NOBOUNDS
        if(i<0 || i>= len(a))
            __throw_index_out_of_range();
#endif
    return i;
}

/* representation */

template<class T> str *__str(T t) { if (!t) return new str("None"); return t->__str__(); }
template<class T> str *repr(T t) { if (!t) return new str("None"); return t->__repr__(); }

str *__str(void *);

/* return pointer to class object */

template<class T> class_ *__type(T t) { return t->__class__; }
template<> class_ *__type(int i);
template<> class_ *__type(double d);

/* equality, comparison */

template<class T> inline __ss_bool __eq(T a, T b) { return ((a&&b)?(a->__eq__(b)):__mbool(a==b)); }
#ifdef __SS_LONG /* XXX */
template<> inline __ss_bool __eq(__ss_int a, __ss_int b) { return __mbool(a == b); }
#endif
template<> inline __ss_bool __eq(str *a, str *b) {
    if(a&&b) {
        if (a->charcache && b->charcache) 
            return __mbool(a==b);
        else
            return __mbool(a->__eq__(b));
    } else
        return __mbool(a==b);
}
template<> inline __ss_bool __eq(int a, int b) { return __mbool(a == b); }
template<> inline __ss_bool __eq(__ss_bool a, __ss_bool b) { return __mbool(a == b); }
template<> inline __ss_bool __eq(double a, double b) { return __mbool(a == b); }
template<> inline __ss_bool __eq(void *a, void *b) { return __mbool(a == b); }

template<class T> inline __ss_bool __ne(T a, T b) { return ((a&&b)?(a->__ne__(b)):__mbool(a!=b)); }
template<> inline __ss_bool __ne(int a, int b) { return __mbool(a != b); }
template<> inline __ss_bool __ne(double a, double b) { return __mbool(a != b); }
template<> inline __ss_bool __ne(void *a, void *b) { return __mbool(a != b); }
template<class T> inline __ss_bool __gt(T a, T b) { return a->__gt__(b); }
template<> inline __ss_bool __gt(int a, int b) { return __mbool(a > b); }
template<> inline __ss_bool __gt(double a, double b) { return __mbool(a > b); }
template<class T> inline __ss_bool __ge(T a, T b) { return a->__ge__(b); }
template<> inline __ss_bool __ge(int a, int b) { return __mbool(a >= b); }
template<> inline __ss_bool __ge(double a, double b) { return __mbool(a >= b); }
template<class T> inline __ss_bool __lt(T a, T b) { return a->__lt__(b); }
template<> inline __ss_bool __lt(int a, int b) { return __mbool(a < b); }
template<> inline __ss_bool __lt(double a, double b) { return __mbool(a < b); }
template<class T> inline __ss_bool __le(T a, T b) { return a->__le__(b); }
template<> inline __ss_bool __le(int a, int b) { return __mbool(a <= b); }
template<> inline __ss_bool __le(double a, double b) { return __mbool(a <= b); }

/* add */

template<class T> inline T __add(T a, T b) { return a->__add__(b); }
#ifdef __SS_LONG
template<> inline __ss_int __add(__ss_int a, __ss_int b) { return a + b; }
#endif
template<> inline int __add(int a, int b) { return a + b; }
template<> inline double __add(double a, double b) { return a + b; }

/* reverse */

template<class U> U __add2(double a, U b) { return b->__add__(a); }
template<class U> U __sub2(double a, U b) { return b->__rsub__(a); }
template<class T> T __mul2(__ss_int n, T a) { return a->__mul__(n); }
template<class T> T __mul2(__ss_bool n, T a) { return a->__mul__(n.value); }
template<class T> T __mul2(double n, T a) { return a->__mul__(n); }
template<class T> T __div2(__ss_int n, T a) { return a->__rdiv__(n); }
template<class T> T __div2(double n, T a) { return a->__rdiv__(n); }

/* copy */

template<class T> T __copy(T t) {
    if(!t)
        return (T)NULL;
    return (T)(t->__copy__());
}

#ifdef __SS_LONG
template<> inline __ss_int __copy(__ss_int i) { return i; }
#endif
template<> inline int __copy(int i) { return i; }
template<> inline __ss_bool __copy(__ss_bool b) { return b; }
template<> inline double __copy(double d) { return d; }
template<> inline void *__copy(void *p) { return p; }

template<class T> T __deepcopy(T t, dict<void *, pyobj *> *memo=0) {
    if(!t)
        return (T)NULL;

    if(!memo)
        memo = new dict<void *, pyobj *>();
    T u = (T)(memo->get(t, 0));
    if(u)
       return u;

    return (T)(t->__deepcopy__(memo));
}

#ifdef __SS_LONG
template<> inline __ss_int __deepcopy(__ss_int i, dict<void *, pyobj *> *) { return i; }
#endif
template<> inline int __deepcopy(int i, dict<void *, pyobj *> *) { return i; }
template<> inline __ss_bool __deepcopy(__ss_bool b, dict<void *, pyobj *> *) { return b; }
template<> inline double __deepcopy(double d, dict<void *, pyobj *> *) { return d; }
template<> inline void *__deepcopy(void *p, dict<void *, pyobj *> *) { return p; }

/* len */

template<class T> inline __ss_int len(T x) { return x->__len__(); }
template<class T> inline __ss_int len(list<T> *x) { return x->units.size(); } /* XXX more general solution? */


/* bool */

inline __ss_int __ss_bool::operator+(__ss_bool b) { return value+b.value; }
inline __ss_bool __ss_bool::operator==(__ss_bool b) { __ss_bool c; c.value=value==b.value; return c; }
inline __ss_bool __ss_bool::operator&(__ss_bool b) { __ss_bool c; c.value=value&b.value; return c; }
inline __ss_bool __ss_bool::operator|(__ss_bool b) { __ss_bool c; c.value=value|b.value; return c; }
inline __ss_bool __ss_bool::operator^(__ss_bool b) { __ss_bool c; c.value=value^b.value; return c; }
inline bool __ss_bool::operator!() { return !value; }
inline __ss_bool::operator bool() { return bool(value); }
inline __ss_bool& __ss_bool::operator=(int a) { value=a; return *this; }

inline __ss_bool ___bool() { return __mbool(false); }

template<class T> inline __ss_bool ___bool(T x) { return __mbool(x && x->__nonzero__()); }
#ifdef __SS_LONG
template<> inline __ss_bool ___bool(__ss_int x) { return __mbool(x!=0); }
#endif
template<> inline __ss_bool ___bool(int x) { return __mbool(x!=0); }
template<> inline __ss_bool ___bool(bool x) { return __mbool(x); }
template<> inline __ss_bool ___bool(__ss_bool x) { return x; }
template<> inline __ss_bool ___bool(double x) { return __mbool(x!=0); }
template<> inline __ss_bool ___bool(void *) { return False; }
template<> inline __ss_bool ___bool(long int) { return False; } /* XXX bool(None) 64-bit */

/* id */

template <class T> __ss_int id(T t) { 
    return (intptr_t)t;
}
template <> __ss_int id(__ss_int);
template <> __ss_int id(double);
template <> __ss_int id(__ss_bool);

/* and, or, not */

#define __OR(a, b, t) ((___bool(__ ## t = a))?(__ ## t):(b))
#define __AND(a, b, t) ((!___bool(__ ## t = a))?(__ ## t):(b))
#define __NOT(x) (__mbool(!x))

/* pyiter methods */

template<class T> inline __iter<T> *pyiter<T>::for_in_init() {
    return this->__iter__();
}

template<class T> inline bool pyiter<T>::for_in_has_next(__iter<T> *iter) {
    iter->__result = iter->__get_next();
    return not iter->__stop_iteration;
}

template<class T> inline T pyiter<T>::for_in_next(__iter<T> *iter) {
    return iter->__result;
}

template<class T> inline __ss_bool pyiter<T>::__contains__(T t) {
    T e;
    typename pyiter<T>::for_in_loop __3;
    int __2;
    pyiter<T> *__1;
    FOR_IN(e,this,1,2,3)
        if(__eq(e,t))
            return __mbool(true);
    END_FOR 
    return __mbool(false);
}

/* pyseq methods */

template<class T> __ss_int pyseq<T>::__cmp__(pyobj *p) {
    if (!p) return 1;
    pyseq<T> *b = (pyseq<T> *)p;
    int i, cmp;
    int mnm = ___min(2, 0, this->__len__(), b->__len__());
    for(i = 0; i < mnm; i++) {
        cmp = __cmp(this->__getitem__(i), b->__getitem__(i));
        if(cmp)
            return cmp;
    }
    return __cmp(this->__len__(), b->__len__());
}

template<class T> __iter<T> *pyseq<T>::__iter__() {
    return new __seqiter<T>(this);
}

template<class T> inline size_t pyseq<T>::for_in_init() {
    return 0;
}

template<class T> inline bool pyseq<T>::for_in_has_next(size_t i) {
    return (__ss_int)i != __len__(); /* XXX opt end cond */
}

template<class T> inline T pyseq<T>::for_in_next(size_t &i) {
    return __getitem__(i++);
}

/* __iter methods */

template<class T> __iter<T> *__iter<T>::__iter__() { 
    __stop_iteration = false; 
    return this; 
}

template<class T> T __iter<T>::next() { /* __get_next can be overloaded instead to avoid (slow) exception handling */
    __result = this->__get_next();
    if(__stop_iteration)
        throw new StopIteration();
    return __result;
} 

template<class T> T __iter<T>::__get_next() {
    try {
        __result = this->next();
    } catch (StopIteration *) {
        __stop_iteration = true;
    }
    return __result;
}

#include "builtin/list.hpp"
#include "builtin/tuple.hpp"
#include "builtin/str.hpp"
#include "builtin/dict.hpp"
#include "builtin/set.hpp"

/* binding args */

#ifdef __SS_BIND
template<class T> T __ss_arg(const char *name, int pos, int has_default, T default_value, PyObject *args, PyObject *kwargs) {
    PyObject *kwarg;
    int nrofargs = PyTuple_Size(args);
    if (pos < nrofargs)
        return __to_ss<T>(PyTuple_GetItem(args, pos));
    else if (kwargs && (kwarg = PyDict_GetItemString(kwargs, name)))
        return __to_ss<T>(kwarg);
    else if (has_default)
        return default_value;
    else
        throw new TypeError(new str("missing argument"));
}
#endif

/* iterators */

template<class T> str *__iter<T>::__repr__() {
    return new str("iterator instance");
}

template<class T> __seqiter<T>::__seqiter() {}
template<class T> __seqiter<T>::__seqiter(pyseq<T> *p) {
    this->p = p;
    size = p->__len__();
    counter = 0;
}

template<class T> T __seqiter<T>::next() {
    if(counter==size)
        __throw_stop_iteration();
    return p->__getitem__(counter++);
}

template<class K, class V> __dictiterkeys<K, V>::__dictiterkeys(dict<K,V> *p) {
    this->p = p;
    this->pos = 0;
    this->si_used = p->used;
}

template<class K, class V> K __dictiterkeys<K, V>::next() {
    if (si_used != p->used) {
        si_used = -1;
        __throw_dict_changed();
    }
    int ret = p->next(&pos, &entry);
    if (!ret) __throw_stop_iteration();
    return entry->key;
}

template<class K, class V> __dictitervalues<K, V>::__dictitervalues(dict<K,V> *p) {
    this->p = p;
    this->pos = 0;
    this->si_used = p->used;
}

template<class K, class V> V __dictitervalues<K, V>::next() {
    if (si_used != p->used) {
        si_used = -1;
        __throw_dict_changed();
    }
    int ret = p->next(&pos, &entry);
    if (!ret) __throw_stop_iteration();
    return entry->value;
}

template<class K, class V> __dictiteritems<K, V>::__dictiteritems(dict<K,V> *p) {
    this->p = p;
    this->pos = 0;
    this->si_used = p->used;
}

template<class K, class V> tuple2<K, V> *__dictiteritems<K, V>::next() {
    if (si_used != p->used) {
        si_used = -1;
        __throw_dict_changed();
    }
    int ret = p->next(&pos, &entry);
    if (!ret) __throw_stop_iteration();
    return new tuple2<K, V>(2, entry->key, entry->value);
}

/* sum */

template<class A> struct __sumtype1 { typedef A type; };
template<> struct __sumtype1<__ss_bool> { typedef int type; };

template<class A, class B> struct __sumtype2 { typedef A type; };
template<> struct __sumtype2<__ss_bool, __ss_bool> { typedef __ss_int type; };
template<> struct __sumtype2<__ss_bool, __ss_int> { typedef __ss_int type; };
template<> struct __sumtype2<__ss_bool, double> { typedef double type; };
template<> struct __sumtype2<__ss_int, double> { typedef double type; };

template <class U> typename __sumtype1<typename U::for_in_unit>::type __sum(U *iter) {
    typename __sumtype1<typename U::for_in_unit>::type result;
    result = (typename __sumtype1<typename U::for_in_unit>::type)0;
    typename U::for_in_unit e;
    typename U::for_in_loop __3;
    int __2;
    U *__1;
    bool first = true;
    FOR_IN(e,iter,1,2,3)
        if(first) {
            result = (typename __sumtype1<typename U::for_in_unit>::type)e;
            first = false;
        }
        else
            result = __add(result, (typename __sumtype1<typename U::for_in_unit>::type)e);
    END_FOR
    return result;
}

template <class U, class B> typename __sumtype2<typename U::for_in_unit,B>::type __sum(U *iter, B b) {
    typename __sumtype1<typename U::for_in_unit>::type result1 = __sum(iter);
    return __add((typename __sumtype2<typename U::for_in_unit,B>::type)b, (typename __sumtype2<typename U::for_in_unit,B>::type)result1);
}

/* max */

template<class A, class B> typename A::for_in_unit ___max(int, B (*key)(typename A::for_in_unit), A *iter) {
    typename A::for_in_unit max;
    B maxkey, maxkey2;
    max = 0; maxkey = 0; maxkey2 = 0;
    int first = 1;
    typename A::for_in_unit e;
    typename A::for_in_loop __3;
    int __2;
    A *__1;
    FOR_IN(e,iter,1,2,3)
        if(key) {
            maxkey2 = key(e);
            if(first || __cmp(maxkey2, maxkey) == 1) {
                max = e;
                maxkey = maxkey2;
            }
        } else if(first || __cmp(e, max) == 1)
            max = e;
        if(first)
            first = 0;
    END_FOR
    if(first)
        throw new ValueError(new str("max() arg is an empty sequence"));
    return max;
}

/* XXX copy-pasto */
template<class A, class B> typename A::for_in_unit ___max(int, pycall1<B, typename A::for_in_unit> *key, A *iter) {
    typename A::for_in_unit max;
    B maxkey, maxkey2;
    int first = 1;
    typename A::for_in_unit e;
    typename A::for_in_loop __3;
    int __2;
    A *__1;
    FOR_IN(e,iter,1,2,3)
        if(key) {
            maxkey2 = key->__call__(e);
            if(first || __cmp(maxkey2, maxkey) == 1) {
                max = e;
                maxkey = maxkey2;
            }
        } else if(first || __cmp(e, max) == 1)
            max = e;
        if(first)
            first = 0;
    END_FOR
    if(first)
        throw new ValueError(new str("max() arg is an empty sequence"));
    return max;
}
template<class A> typename A::for_in_unit ___max(int nn, int, A *iter) { return ___max(nn, (int (*)(typename A::for_in_unit))0, iter); }

template<class T, class B> inline T ___max(int, B (*key)(T), T a, T b) { return (__cmp(key(a), key(b))==1)?a:b; }
template<class T> inline  T ___max(int, int, T a, T b) { return (__cmp(a, b)==1)?a:b; }

template<class T, class B> T ___max(int n, B (*key)(T), T a, T b, T c, ...) {
    T m = ___max(2, key, ___max(2, key, a, b), c);
    B maxkey = key(m);
    va_list ap;
    va_start(ap, c);
    for(int i=0; i<n-3; i++) {
        T t = va_arg(ap, T);
        if(__cmp(key(t),maxkey)==1)
            m=t;
    }
    va_end(ap);
    return m;
}
template<class T> T ___max(int n, int key, T a, T b, T c, ...) {
    T m = ___max(2, key, ___max(2, key, a, b), c);
    va_list ap;
    va_start(ap, c);
    for(int i=0; i<n-3; i++) {
        T t = va_arg(ap, T);
        if(__cmp(t,m)==1) m=t;
    }
    va_end(ap);
    return m;
}

/* min */

template<class A, class B> typename A::for_in_unit ___min(int, B (*key)(typename A::for_in_unit), A *iter) {
    typename A::for_in_unit min;
    B minkey, minkey2;
    min = 0; minkey = 0; minkey2 = 0;
    int first = 1;
    typename A::for_in_unit e;
    typename A::for_in_loop __3;
    int __2;
    A *__1;
    FOR_IN(e,iter,1,2,3)
        if(key) {
            minkey2 = key(e);
            if(first || __cmp(minkey2, minkey) == -1) {
                min = e;
                minkey = minkey2;
            }
        } else if(first || __cmp(e, min) == -1)
            min = e;
        if(first)
            first = 0;
    END_FOR
    if(first)
        throw new ValueError(new str("min() arg is an empty sequence"));
    return min;
}
template<class A> typename A::for_in_unit ___min(int nn, int, A *iter) { return ___min(nn, (int (*)(typename A::for_in_unit))0, iter); }

template<class T, class B> inline T ___min(int, B (*key)(T), T a, T b) { return (__cmp(key(a), key(b))==-1)?a:b; }
template<class T> inline  T ___min(int, int, T a, T b) { return (__cmp(a, b)==-1)?a:b; }

template<class T, class B> T ___min(int n, B (*key)(T), T a, T b, T c, ...) {
    T m = ___min(2, key, ___min(2, key, a, b), c);
    B minkey = key(m);
    va_list ap;
    va_start(ap, c);
    for(int i=0; i<n-3; i++) {
        T t = va_arg(ap, T);
        if(__cmp(key(t),minkey)==-1)
            m=t;
    }
    va_end(ap);
    return m;
}
template<class T> T ___min(int n, int key, T a, T b, T c, ...) { /* XXX */
    T m = ___min(2, key, ___min(2, key, a, b), c);
    va_list ap;
    va_start(ap, c);
    for(int i=0; i<n-3; i++) {
        T t = va_arg(ap, T);
        if(__cmp(t,m)==-1)
            m=t;
    }
    va_end(ap);
    return m;
}

/* sorted */

template <class U, class V, class W> list<typename U::for_in_unit> *sorted(U *iter, V cmp, W key, __ss_int reverse) {
    typename U::for_in_unit e;
    typename U::for_in_loop __3;
    int __2;
    U *__1;
    list<typename U::for_in_unit> *l = new list<typename U::for_in_unit>();
    FOR_IN(e,iter,1,2,3)
        l->units.push_back(e);
    END_FOR
    l->sort(cmp, key, reverse);
    return l;
}

template <class A, class V, class W> list<A> *sorted(list<A> *x, V cmp, W key, __ss_int reverse) {
    list<A> *l = new list<A>();
    l->units = x->units;
    l->sort(cmp, key, reverse);
    return l;
}

template <class A, class V, class W> list<A> *sorted(tuple2<A,A> *x, V cmp, W key, __ss_int reverse) {
    list<A> *l = new list<A>();
    l->units = x->units;
    l->sort(cmp, key, reverse);
    return l;
}

template <class V, class W> list<str *> *sorted(str *x, V cmp, W key, __ss_int reverse) {
    list<str *> *l = new list<str *>(x);
    l->sort(cmp, key, reverse);
    return l;
}

/* reversed */

template<class A> class __ss_reverse : public __iter<A> {
public:
    pyseq<A> *p;
    __ss_int i;
    __ss_reverse(pyseq<A> *p) {
        this->p = p;
        i = len(p);
    }

    A __get_next() {
        if(i>0)
            return p->__getitem__(--i); /* XXX avoid wrap, str spec? */
        this->__stop_iteration = true;
    }
};

template <class A> __ss_reverse<A> *reversed(pyiter<A> *x) {
    return new __ss_reverse<A>(new list<A>(x));
}
template <class A> __ss_reverse<A> *reversed(pyseq<A> *x) {
    return new __ss_reverse<A>(x);
}
__iter<__ss_int> *reversed(__xrange *x);

/* enumerate */

template<class A> class __enumiter : public __iter<tuple2<__ss_int, A> *> {
public:
    __iter<A> *p;
    __ss_int i;

    __enumiter(pyiter<A> *p) {
        this->p = ___iter(p);
        i = 0;
    }

    tuple2<__ss_int, A> *next() {
        return new tuple2<__ss_int, A>(2, i++, p->next());
    }
};

template <class A> __iter<tuple2<__ss_int, A> *> *enumerate(pyiter<A> *x) {
    return new __enumiter<A>(x);
}

/* zip */

list<tuple2<void *, void *> *> *__zip(int nn);

template <class A> list<tuple2<typename A::for_in_unit, typename A::for_in_unit> *> *__zip(int nn, A *iter) {
    list<tuple2<typename A::for_in_unit, typename A::for_in_unit> *> *result = (new list<tuple2<typename A::for_in_unit, typename A::for_in_unit> *>());
    typename A::for_in_unit e;
    typename A::for_in_loop __3;
    int __2;
    A *__1;
    FOR_IN(e,iter,1,2,3)
        result->append((new tuple2<typename A::for_in_unit, typename A::for_in_unit>(1, e)));
    END_FOR
    return result;
}

template <class A, class B> list<tuple2<typename A::for_in_unit, typename B::for_in_unit> *> *__zip(int, A *itera, B *iterb) {
    list<tuple2<typename A::for_in_unit, typename B::for_in_unit> *> *result = (new list<tuple2<typename A::for_in_unit, typename B::for_in_unit> *>());
    tuple2<typename A::for_in_unit, typename B::for_in_unit> *tuples;
    int count = -1;
    if(A::is_pyseq && B::is_pyseq) {
        count = __SS_MIN(len(itera), len(iterb));
        tuples = new tuple2<typename A::for_in_unit, typename B::for_in_unit>[count];
        result->units.resize(count);
    }
    typename A::for_in_unit e;
    typename A::for_in_loop __3 = itera->for_in_init();
    typename B::for_in_unit f;
    typename B::for_in_loop __6 = iterb->for_in_init();
    int i = 0;
    while(itera->for_in_has_next(__3) and iterb->for_in_has_next(__6)) {
        e = itera->for_in_next(__3);
        f = iterb->for_in_next(__6);
        if(count == -1)
            result->append((new tuple2<typename A::for_in_unit, typename B::for_in_unit>(2, e, f)));
        else {
            tuples[i].__init2__(e, f);
            result->units[i] = &tuples[i];
            i++;
        }
    }
    return result;
}

template <class A, class B, class C> list<tuple2<typename A::for_in_unit, typename A::for_in_unit> *> *__zip(int, A *itera, B *iterb, C *iterc) {
    list<tuple2<typename A::for_in_unit, typename A::for_in_unit> *> *result = (new list<tuple2<typename A::for_in_unit, typename A::for_in_unit> *>());
    tuple2<typename A::for_in_unit, typename A::for_in_unit> *tuples;
    int count = -1;
    if(A::is_pyseq && B::is_pyseq && C::is_pyseq) {
        count = __SS_MIN3(len(itera), len(iterb), len(iterc));
        tuples = new tuple2<typename A::for_in_unit, typename A::for_in_unit>[count];
        result->units.resize(count);
    }
    typename A::for_in_unit e;
    typename A::for_in_loop __3 = itera->for_in_init();
    typename B::for_in_unit f;
    typename B::for_in_loop __6 = iterb->for_in_init();
    typename C::for_in_unit g;
    typename C::for_in_loop __7 = iterc->for_in_init();
    int i = 0;
    while(itera->for_in_has_next(__3) and iterb->for_in_has_next(__6) and iterc->for_in_has_next(__7)) {
        e = itera->for_in_next(__3);
        f = iterb->for_in_next(__6);
        g = iterc->for_in_next(__7);
        if(count == -1)
            result->append((new tuple2<typename A::for_in_unit, typename A::for_in_unit>(3, e, f, g)));
        else {
            tuples[i].units.push_back(e);
            tuples[i].units.push_back(f);
            tuples[i].units.push_back(g);
            result->units[i] = &tuples[i];
            i++;
        }
    }
    return result;
}

/* next */

template <class A> A next(__iter<A> *iter1, A fillvalue) {
    try {
        return iter1->next();
    } catch(StopIteration *) {
        return fillvalue;
    }
}
template <class A> A next(__iter<A> *iter1, void *) { return next(iter1, (A)NULL); }
template <class A> A next(__iter<A> *iter1) { return iter1->next(); }

/* map */

template <class A, class B> list<B> *map(int, B (*func)(typename A::for_in_unit), A *iter) {
    if(!func)
        throw new ValueError(new str("'map' function argument cannot be None"));
    list<B> *result = new list<B>();
    int count = -1;
    if(A::is_pyseq) {
        count = len(iter);
        result->units.resize(count);
    }
    typename A::for_in_unit e;
    typename A::for_in_loop __3 = iter->for_in_init();
    int i = 0;
    while(iter->for_in_has_next(__3)) {
        e = iter->for_in_next(__3);
        if(count == -1)
            result->append((*func)(e));
        else
            result->units[i++] = (*func)(e);
    }
    return result;
}

template <class A, class B, class C> list<A> *map(int n, A (*func)(B, C), pyiter<B> *b, pyiter<C> *c) {
    if(!func)
        throw new ValueError(new str("'map' function argument cannot be None"));
    list<A> *result = new list<A>();
    __iter<B> *itb = b->__iter__();
    __iter<C> *itc = c->__iter__();
    B nextb;
    C nextc;
    int total;
    while(1) {
        total = 0;
        try { nextb = next(itb); total += 1; } catch (StopIteration *) { nextb = 0; }
        try { nextc = next(itc); total += 1; } catch (StopIteration *) { nextc = 0; }
        if(total == 0)
            break;
        result->append((*func)(nextb, nextc));
    }
    return result;
}

template <class A, class B, class C, class D> list<A> *map(int, A (*func)(B, C, D), pyiter<B> *b1, pyiter<C> *b2, pyiter<D> *b3) {
    if(!func)
        throw new ValueError(new str("'map' function argument cannot be None"));
    list<A> *result = new list<A>();
    __iter<B> *itb1 = b1->__iter__();
    __iter<C> *itb2 = b2->__iter__();
    __iter<D> *itb3 = b3->__iter__();
    B nextb1;
    C nextb2;
    D nextb3;
    int total;
    while(1)  {
        total = 0;
        try { nextb1 = next(itb1); total += 1; } catch (StopIteration *) { nextb1 = 0; }
        try { nextb2 = next(itb2); total += 1; } catch (StopIteration *) { nextb2 = 0; }
        try { nextb3 = next(itb3); total += 1; } catch (StopIteration *) { nextb3 = 0; }
        if(total == 0)
            break;
        result->append((*func)(nextb1, nextb2, nextb3));
    }
    return result;
}

/* reduce */

template<class A> typename A::for_in_unit reduce(typename A::for_in_unit (*func)(typename A::for_in_unit, typename A::for_in_unit), A *iter, typename A::for_in_unit initial) {
    typename A::for_in_unit result = initial;
    typename A::for_in_loop __7 = iter->for_in_init();
    while(iter->for_in_has_next(__7))
        result = (*func)(result, iter->for_in_next(__7));
    return result;
}

template<class A> typename A::for_in_unit reduce(typename A::for_in_unit (*func)(typename A::for_in_unit, typename A::for_in_unit), A *iter) {
    typename A::for_in_unit result;
    typename A::for_in_loop __7 = iter->for_in_init();
    int first = 1;
    while(iter->for_in_has_next(__7)) {
        if(first) {
            result = iter->for_in_next(__7);
            first = 0;
        } else
            result = (*func)(result, iter->for_in_next(__7));
    }
    if(first) 
        throw new TypeError(new str("reduce() of empty sequence with no initial value"));
    return result;
}

/* filter */

template <class A, class B> list<typename A::for_in_unit> *filter(B (*func)(typename A::for_in_unit), A *iter) {
    list<typename A::for_in_unit> *result = new list<typename A::for_in_unit>();
    typename A::for_in_unit e;
    typename A::for_in_loop __3 = iter->for_in_init();
    while(iter->for_in_has_next(__3)) {
        e = iter->for_in_next(__3);
        if(func) {
            if(___bool((*func)(e)))
                result->append(e);
        } else if(___bool(e))
            result->append(e);
    }
    return result;
}

template <class A, class B> tuple2<A,A> *filter(B (*func)(A), tuple2<A,A> *a) {
    tuple2<A,A> *result = new tuple2<A,A>();
    int size = len(a);
    A e;
    for(int i=0; i<size; i++) {
        e = a->units[i];
        if(func) {
            if(___bool((*func)(e)))
                result->units.push_back(e);
        } else if(___bool(e))
            result->units.push_back(e);
    }
    return result;
}

template <class B> str *filter(B (*func)(str *), str *a) {
    str *result = new str();
    int size = len(a);
    char e;
    str *c;
    for(int i=0; i<size; i++) {
        e = a->unit[i];
        if(func) {
            c = __char_cache[((unsigned char)e)];
            if(___bool((*func)(c)))
                result->unit.push_back(e);
        } else 
            result->unit.push_back(e);
    }
    return result;
}

template <class A> list<A> *filter(void *func, pyiter<A> *a) { return filter(((int(*)(A))(func)), a); }
inline str *filter(void *, str *a) { return filter(((int(*)(str *))(0)), a); }
template <class A> tuple2<A,A> *filter(void *func, tuple2<A,A> *a) { return filter(((int(*)(A))(func)), a); }

/* pow */

template<class A, class B> double __power(A a, B b);
template<> inline double __power(__ss_int a, double b) { return pow(a,b); }
template<> inline double __power(double a, __ss_int b) { 
    if(b==2) return a*a;
    else if(b==3) return a*a*a;
    else return pow(a,b); 
}

complex *__power(complex *a, complex *b);
complex *__power(complex *a, __ss_int b);
complex *__power(complex *a, double b);

template<class A> A __power(A a, A b);
template<> inline double __power(double a, double b) { return pow(a,b); }

template<> inline __ss_int __power(__ss_int a, __ss_int b) {
    switch(b) {
        case 2: return a*a;
        case 3: return a*a*a;
        case 4: return a*a*a*a;
        case 5: return a*a*a*a*a;
        case 6: return a*a*a*a*a*a;
        case 7: return a*a*a*a*a*a*a;
        case 8: return a*a*a*a*a*a*a*a;
        case 9: return a*a*a*a*a*a*a*a*a;
        case 10: return a*a*a*a*a*a*a*a*a*a;
    }
    __ss_int res, tmp;

    res = 1;
    tmp = a;

    while((b>0)) {
        if ((b%2)) {
            res = (res*tmp);
        }
        tmp = (tmp*tmp);
        b = (b/2);
    }
    return res;
}

#ifdef __SS_LONG
inline __ss_int __power(__ss_int a, __ss_int b, __ss_int c) {
    __ss_int res, tmp;

    res = 1;
    tmp = a;

    while((b>0)) {
        if ((b%2)) {
            res = ((res*tmp)%c);
        }
        tmp = ((tmp*tmp)%c);
        b = (b/2);
    }
    return res;
}
#endif

inline int __power(int a, int b, int c) {
    int res, tmp;

    res = 1;
    tmp = a;

    while((b>0)) {
        if ((b%2)) {
            res = ((res*tmp)%c);
        }
        tmp = ((tmp*tmp)%c);
        b = (b/2);
    }
    return res;
}

/* division */

template<class A, class B> double __divs(A a, B b);
template<> inline double __divs(__ss_int a, double b) { return (double)a/b; }
template<> inline double __divs(double a, __ss_int b) { return a/((double)b); }

template<class A> A __divs(A a, A b);
template<> inline double __divs(double a, double b) { return a/b; }
#ifdef __SS_LONG
template<> inline __ss_int __divs(__ss_int a, __ss_int b) {
    if(a<0 && b>0) return (a-b+1)/b;
    else if(b<0 && a>0) return (a-b-1)/b;
    else return a/b;
}
#endif
template<> inline int __divs(int a, int b) {
    if(a<0 && b>0) return (a-b+1)/b;
    else if(b<0 && a>0) return (a-b-1)/b;
    else return a/b;
}

template<class A, class B> double __floordiv(A a, B b);
template<> inline double __floordiv(__ss_int a, double b) { return floor((double)a/b); }
template<> inline double __floordiv(double a, __ss_int b) { return floor(a/((double)b)); }

template<class A> inline A __floordiv(A a, A b) { return a->__floordiv__(b); }
template<> inline double __floordiv(double a, double b) { return floor(a/b); }

#ifdef __SS_LONG /* XXX */
template<> inline __ss_int __floordiv(__ss_int a, __ss_int b) { return (__ss_int)floor((double)a/b); } /* XXX */
#endif
template<> inline int __floordiv(int a, int b) { return (int)floor((double)a/b); } /* XXX */

/* modulo */

template<class A> A __mods(A a, A b);
#ifdef __SS_LONG /* XXX */
template<> inline __ss_int __mods(__ss_int a, __ss_int b) {
    int m = a%b;
    if((m<0 && b>0)||(m>0 && b<0)) m+=b;
    return m;
}
#endif
template<> inline int __mods(int a, int b) {
    int m = a%b;
    if((m<0 && b>0)||(m>0 && b<0)) m+=b;
    return m;
}
template<> inline double __mods(double a, double b) {
    double f = fmod(a,b);
    if((f<0 && b>0)||(f>0 && b<0)) f+=b;
    return f;
}

template<class A, class B> double __mods(A a, B b);
#ifdef __SS_LONG
template<> inline double __mods(__ss_int a, double b) { return __mods((double)a, b); }
template<> inline double __mods(double a, __ss_int b) { return __mods(a, (double)b); }
#endif
template<> inline double __mods(int a, double b) { return __mods((double)a, b); }
template<> inline double __mods(double a, int b) { return __mods(a, (double)b); }

/* divmod */

template<class A> inline tuple2<A, A> *divmod(A a, A b) { return a->__divmod__(b); }
template<> inline tuple2<double, double> *divmod(double a, double b) {
    return new tuple2<double, double>(2, __floordiv(a,b), __mods(a,b));
}
#ifdef __SS_LONG
template<> inline tuple2<__ss_int, __ss_int> *divmod(__ss_int a, __ss_int b) {
    return new tuple2<__ss_int, __ss_int>(2, __floordiv(a,b), __mods(a,b));
}
#endif
template<> inline tuple2<int, int> *divmod(int a, int b) {
    return new tuple2<int, int>(2, __floordiv(a,b), __mods(a,b));
}
template<class A, class B> tuple2<double, double> *divmod(A a, B b);
#ifdef __SS_LONG
template<> inline tuple2<double, double> *divmod(double a, __ss_int b) { return divmod(a, (double)b); }
template<> inline tuple2<double, double> *divmod(__ss_int a, double b) { return divmod((double)a, b); }
#endif
template<> inline tuple2<double, double> *divmod(double a, int b) { return divmod(a, (double)b); }
template<> inline tuple2<double, double> *divmod(int a, double b) { return divmod((double)a, b); }

tuple2<complex *, complex *> *divmod(complex *a, double b);
tuple2<complex *, complex *> *divmod(complex *a, __ss_int b);

/* dict.fromkeys */

namespace __dict__ {
    template<class A, class B> dict<A, B> *fromkeys(pyiter<A> *f, B b) {
        dict<A, B> *d = new dict<A, B>();
        typename pyiter<A>::for_in_unit e;
        typename pyiter<A>::for_in_loop __3;
        int __2;
        pyiter<A> *__1;
        FOR_IN(e,f,1,2,3)
            d->__setitem__(e, b);
        END_FOR
        return d;
    }

    template<class A> dict<A, void *> *fromkeys(pyiter<A> *f) {
        return fromkeys(f, (void *)0);
    }

}

/* string formatting */

int __fmtpos(str *fmt);
int __fmtpos2(str *fmt);
void __modfill(str **fmt, pyobj *t, str **s);
str *mod_to_c2(pyobj *t);
int_ *mod_to_int(pyobj *t);
float_ *mod_to_float(pyobj *t);

template<class T> str *__modtuple(str *fmt, tuple2<T,T> *t) {
    list<pyobj *> *vals = new list<pyobj *>();
    for(int i=0;i<len(t);i++)
        vals->append(___box(t->__getitem__(i)));
    return __mod4(fmt, vals);
}

template<class A, class B> str *__modtuple(str *fmt, tuple2<A,B> *t) {
    list<pyobj *> *vals = new list<pyobj *>(2, ___box(t->__getfirst__()), ___box(t->__getsecond__()));
    return __mod4(fmt, vals);
}

template<class T> str *__moddict(str *v, dict<str *, T> *d) {
    str *const_6 = new str(")");
    int i, pos, pos2;
    list<str *> *names = (new list<str *>());

    while((pos = __fmtpos2(v)) != -1) {
        pos2 = v->find(const_6, pos);
        names->append(v->__slice__(3, (pos+2), pos2, 0));
        v = (v->__slice__(2, 0, (pos+1), 0))->__add__(v->__slice__(1, (pos2+1), 0, 0));
    }

    list<pyobj *> *vals = new list<pyobj *>();
    for(i=0;i<len(names);i++)
        vals->append(___box(d->__getitem__(names->__getitem__(i))));
    return __mod4(v, vals);
}

/* boxing */

template<class T> T ___box(T t) { return t; } /* XXX */
#ifdef __SS_LONG
int_ *___box(__ss_int);
#endif
int_ *___box(int);
int_ *___box(unsigned int); /* XXX */
int_ *___box(unsigned long);
int_ *___box(unsigned long long);
bool_ *___box(__ss_bool);
float_ *___box(double);

/* any */

template<class A> __ss_bool any(A *iter) {
    typename A::for_in_unit e;
    typename A::for_in_loop __3;
    int __2;
    A *__1;
    FOR_IN(e,iter,1,2,3)
        if(___bool(e))
            return True;
    END_FOR
    return False;
}

/* all */

template<class A> __ss_bool all(A *iter) {
    typename A::for_in_unit e;
    typename A::for_in_loop __3;
    int __2;
    A *__1;
    FOR_IN(e,iter,1,2,3)
        if(!___bool(e))
            return False;
    END_FOR
    return True;
}

/* ord, chr */

int ord(str *c);

static void __throw_chr_out_of_range() { /* improve inlining */
    throw new ValueError(new str("chr() arg not in range(256)"));
}
inline str *chr(int i) {
    if(i < 0 || i > 255)
        __throw_chr_out_of_range();
    return __char_cache[i];
}
inline str *chr(__ss_bool b) { return chr(b.value); }

template<class T> inline str *chr(T t) {
    return chr(t->__int__());
}

#ifdef __SS_LONG
inline str *chr(__ss_int i) {
    return chr((int)i);
}

template<> inline str *hex(__ss_int i) {
    return hex((int)i);
}
template<> inline str *oct(__ss_int i) {
    return oct((int)i);
}
template<> inline str *bin(__ss_int i) {
    return bin((int)i);
}
#endif

/* complex */

template<class T> complex::complex(T t) {
    __class__ = cl_complex;
    real = __float(t);
    imag = 0;
}

#ifdef __SS_BIND
PyObject *__ss__newobj__(PyObject *, PyObject *args, PyObject *kwargs);
#endif

/* slicing */

static void inline slicenr(__ss_int x, __ss_int &l, __ss_int &u, __ss_int &s, __ss_int len) {
    if(x&4) {
        if (s == 0)
            __throw_slice_step_zero();
    } else
        s = 1;

    if (l>=len)
        l = len;
    else if (l<0) {
        l = len+l;
        if(l<0)
            l = 0;
    }
    if (u>=len)
        u = len;
    else if (u<0) {
        u = len+u;
        if(u<0)
            u = 0;
    }

    if(s<0) {
        if (!(x&1))
            l = len-1;
        if (!(x&2))
            u = -1;
    }
    else {
        if (!(x&1))
            l = 0;
        if (!(x&2))
            u = len;
    }
}

/* file */

template<class U> void *file::writelines(U *iter) {
    __check_closed();
    typename U::for_in_unit e;
    typename U::for_in_loop __3;
    int __2;
    U *__1;
    FOR_IN(e,iter,1,2,3)
        write(e);
    END_FOR
    return NULL;
}

} // namespace __shedskin__
#endif
