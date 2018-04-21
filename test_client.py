import socket
import random
import hashlib

PORT = 2345
ADDR = 'localhost'
test_usr = 'test'
test_usr_pwd = 'test'

def randomStr(n):
    s = ''
    for i in range(n):
        s += chr(random.randint(0, 25)+ord('a'))
    return s

def signup(usrname, pwd, conn):
    hashobj = hashlib.sha256((usrname+pwd).encode())
    conn.send(b'1')
    conn.send(usrname.encode()+b'\n')
    conn.send(hashobj.digest())
    if conn.recv(1) != b'1':
        raise Exception("signup failed")   

def login(usrname, pwd, conn):
    hashobj = hashlib.sha256((usrname+pwd).encode())
    conn.send(b'2')
    salt = conn.recv(8)
    conn.send(usrname.encode()+b'\n')
    conn.send(hashlib.sha256(hashobj.digest()+salt).digest())
    if (conn.recv(1) != b'1'):
        raise Exception("login failed")  

def signup_login_test(): 
    conn = socket.socket() 
    conn.connect((ADDR, PORT))
    usrname = randomStr(random.randint(5, 15))
    pwd = randomStr(random.randint(5, 15))
    try:
        signup(usrname, pwd, conn)
        login(usrname, pwd, conn)
        conn.send(b'0')
    except Exception as e:
        print(e)
    else:
        print("signup and login test success")
    finally:
        conn.close()

def upload_a_book(book, conn):
    conn.send(b'3')
    conn.send(int(1).to_bytes(4, 'little'))
    conn.send(book.encode()+b'\n')
    conn.send(hashlib.sha256(book.encode()).digest())
    if (conn.recv(1) != b'1'):
        raise Exception("upload failed")

def upload_test():
    conn = socket.socket() 
    conn.connect((ADDR, PORT))
    book = randomStr(10)
    try:
        login(test_usr, test_usr_pwd, conn)
        upload_a_book(book, conn)
    except Exception as e:
        print(e)
    else:
        print("upload test success")
    finally:
        conn.close()   

if __name__ == '__main__':
    signup_login_test()
    upload_test()
    