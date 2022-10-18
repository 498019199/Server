package rpc

import (
	"encoding/json"
	"io"
	"log"
	"net"
)

type Server struct{}

const MagicNumber = 0x3bef5c

type Option struct {
	MagicNumber int        // MagicNumber marks this's a geerpc request
	Type        Proxy. // client may choose different Codec to encode body
}

var DefaultOption = &Option{
	MagicNumber: MagicNumber,
	CodecType:   codec.GobType,
}

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

	defer func() { _ = conn.Close() }()
	var opt Option
	if err := json.NewDecoder(conn).Decode(&opt); err != nil {
		log.Println("rpc server: options error: ", err)
		return
	}
	if opt.MagicNumber != MagicNumber {
		log.Printf("rpc server: invalid magic number %x", opt.MagicNumber)
		return
	}
	f := codec.NewCodecFuncMap[opt.CodecType]
	if f == nil {
		log.Printf("rpc server: invalid codec type %s", opt.CodecType)
		return
	}
	server.serveCodec(f(conn))
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
