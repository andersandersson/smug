#define JCALL0(env, method) (*env)->method(env)
#define JCALL1(env, method, a1) (*env)->method(env, a1)
#define JCALL3(env, method, a1, a2, a3) (*env)->method(env, a1, a2, a3)
