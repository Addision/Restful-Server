#!/usr/bin/env python
# -*- coding: utf-8 -*-
from flask import Flask, request
from flask_restful import reqparse, abort, Api, Resource
from BookList import *

import os,sys
import json


app = Flask(__name__)
api = Api(app)

BOOKS = {

}

booklist = BookList()


def abort_if_book_not_exist(book_id):
    if book_id not in BOOKS:
        abort(404, message="Book {} doesn't exist".format(book_id))


parser = reqparse.RequestParser()
parser.add_argument('id')
parser.add_argument('name')
parser.add_argument('author')
parser.add_argument('content')

def Store(book,path):
    fileName = book['id']+'.json'
    filePath = os.path.join(path, fileName)
    print("path====", filePath)
    with open(filePath, "w+", encoding="utf-8") as file:
        file.write(json.dumps(book))

class DoBook(Resource):
    def post(self, book_id):
        args = parser.parse_args()
        book_id = args['id']
        book_name = args['name']
        book_author = args['author']
        book_content = args['content']
        BOOKS[book_id] = {}
        BOOKS[book_id]['id'] = book_id
        BOOKS[book_id]['name'] = book_name
        BOOKS[book_id]['author'] = book_author
        BOOKS[book_id]['content'] = book_content
        booklist.Insert(book_id)
        #写文件持久化
        Store(BOOKS[book_id], './library/')
        return book_id, BOOKS[book_id]

    def get(self, book_id):
        if book_id not in booklist:
            abort(404, message="Book {} doesn't exist".format(book_id))
        return BOOKS[book_id]
    
class BooksList(Resource):
    def get(self):
        return BOOKS


##
## Actually setup the Api resource routing here
##
api.add_resource(BooksList, '/bookslist')
api.add_resource(DoBook, '/api/v1/books/<book_id>')

def ReadFiles(filepath):
    listFiles = os.listdir(filepath)
    if not listFiles:
        return
    for file in listFiles:
        childFilePath = filepath+file
        if os.path.isfile(childFilePath):
            book_id = (os.path.split(file)[1]).split('.')[0]
            BOOKS[book_id] = {}
            book_info = ReadFile(childFilePath)
            # print(book_info)
            BOOKS[book_id] = book_info
            booklist.Insert(book_id)
            # print(booklist)
            
def ReadFile(fileName):
    with open(fileName, encoding = 'utf-8') as file:
        return json.load(file)
    return ""    

if __name__ == '__main__':
    ReadFiles('./library/')
    app.run(host="0.0.0.0", port=5555, debug=True)
    
    
#TODO：可以利用protobuf序列化书籍 减小磁盘保存空间