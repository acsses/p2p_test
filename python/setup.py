from setuptools import setup, Extension

http = Extension('p2ptest',
                    sources = ['src/pyhttp.c','../p2ptest/http.c','../p2ptest/parser.c','../p2ptest/timeutil.c'],
                    libraries=["expat","json-c","ssl","crypto"],
                    library_dirs=["/usr/local/opt/expat/lib", "/usr/local/opt/json-c/lib","/usr/local/opt/openssl@3.1/lib"],
                    include_dirs=["/usr/local/opt/expat/include","/usr/local/opt/json-c/include","usr/local/opt/openssl@3.1/include"],
                    extra_compile_args=['-lexpat',"-ljson-c","-lssl","-lcrypto"]
                    #extra_objects = ['hello.o'],
                    )

setup(name = 'p2ptest', version = '1.0.0',
    ext_modules = [http],
)
