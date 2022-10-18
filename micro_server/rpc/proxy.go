package rpc

import "io"

type Header struct {
	ServiceMothod string // 服务名.方法名，通常与 Go 语言中的结构体和方法相映射。
	Seq           int64  // 请求的序号,用来区分不同的请求。
	Error         string // 是错误信息，客户端置为空，服务端如果如果发生错误，将错误信息置于 Error 中
}

type Proxy interface {
	io.Closer
	ReadHeader(header *Header) error
	ReadBody(interface{}) error
	Write(*Header, interface{}) error
}

type Type string

const (
	GobType  Type = "application/gob"
	JsonType Type = "application/json" // not implemented
)

type NewProxyFunc func(io.ReadWriteCloser) Proxy

var NewProxyFuncMap map[Type]NewProxyFunc

func init() {

	NewProxyFuncMap = make(map[string]NewProxyFunc)
	NewProxyFuncMap[GobType] = NewGobProxy
}
