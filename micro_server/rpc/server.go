package rpc

import (
	"io"
	"log"
	"net"
)

type Server struct{}

func NewServer() *Server {
	return &Server{}
}

var DefaultServer = NewServer()

// 连接服务器请求
func (server *Server) Accept(lis net.Listener) {
	for {
		conn, err := lis.Accept()
		if err != nil {
			log.Println("rpc server :accept error:", err)
		}

		go server.ServerConn(conn)
	}
}

// 新连接
func (server *Server) ServerConn(conn io.ReadWriteCloser) {

}

//读取请求
func (server *Server) ReadRequest(conn io.ReadWriteCloser) {

}

// 处理请求
func (server *Server) handleRequest(conn io.ReadWriteCloser) {

}

// 回复请求
func (server *Server) sendResponse(conn io.ReadWriteCloser) {

}
