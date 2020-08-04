#ifndef LOGINLIB_H
#define LOGINLIB_H


class LoginLib
{

public:
    LoginLib();
    ~LoginLib();
    static LoginLib *Instance();  // 单实例
    void showLoginDialog();       // 显示登陆对话框

private:
    static LoginLib *m_Instance;  // 实例对象
};

#endif // LOGINLIB_H
