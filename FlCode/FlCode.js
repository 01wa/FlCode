/*
模块：分离码

功能：
	分离码是一种信息编解码技术，主要利用数学的不同进制转换来形成，结合码表和数学的进制转换，提出码位分离的编解码方法。
版权：
   上海泥娃通信科技有限公司
   email: zhangliuxue@126.com
*/


//初始化
function InitJz(st)
{
    st.iCodeTable = new Object();
    if (st.letters == undefined || st.letters=="")
        //st.letters = "0123456789abcdefghijklmnopqrstuv";
        st.letters = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-";
    st.jz = st.letters.length;
    for (var i = 0; i < st.jz; i++)
    {
        st.iCodeTable[st.letters[i]] = i;
    }
}
//获取字符串表示的整数
function GetJzInt(st,cValue,offset,  iL)
{
    var i = 0;
    for ( k =0; k<iL; k++)
    {
        i = i + st.iCodeTable[cValue[offset+k]] * Math.pow(st.jz, k);
    }
    return i;
}
//获取整数表示的字符串，并给出字符串的长度
function GetJzChar(st,iValue)
{
    var i = iValue;
    var p = "";
    var k = 0;
    while (i != 0)
    {
        i = parseInt(iValue / st.jz);
        p += st.letters[parseInt(iValue % st.jz)];
        
        iValue = i;
        if (i >= st.jz) {
            k++;
        }else 
        {
            p += st.letters[parseInt(iValue % st.jz)];
            k++;
            break;
        }
    }
    
    k++;
    var e = {};
    e.str = p;
    e.m = k;
    return e;
}
function chartoint(c,st,len)
{
    var iv="";
    for(var i=st+len-1;i>=st;i--)
    {
        iv += c[i];
    }
    return parseInt(iv,16);
}
/*
编码：	输入消息，转换为字符串序列和位数序列
*/
function EncodeJz(st,msg,iSer)
{
    var len = msg.length;
    var i = Math.floor(len / 4);
    var j = len % 4;
    var outstr="";
    for (var k = 0; k < i; k++)
    {
        var p =chartoint(msg,k*4,4);
        var m = 0;
        if (p != NaN)
        {
            var tmp = GetJzChar(st, p);
            outstr += tmp.str;
            iSer.push(tmp.m);
        }
    }
    if (j>0)
    {
        var p =chartoint(msg,k*4,j);
        var m = 0;
        var tmp = GetJzChar(st, p);
        outstr += tmp.str;
        iSer.push(tmp.m);
    }
    return outstr;
}
/*
解码
*/
function DecodeJz( st, msg, len, iSer)
{
    var p1="";
    var offset = 0;
    
    for (x in iSer)
    {
        var m = GetJzInt(st, msg, offset, iSer[x]);
        var p = m.toString(16);
        m = "";
        for (var i=0;i<p.length/2;i++)
        {
            var tmp = p.substr(i * 2, 2);
            m = tmp +m;
        }
		p1 += m.toString(16);
		offset += iSer[x];
    }
    return  p1;
}
function Encode16(str)
{
    var rstr=""
    for (i = 0; i < str.length; i++) {
        if(str.charAt(i)=='%')
        {
            rstr += str.charAt(i + 1);
            rstr += str.charAt(i + 2);
            i = i + 2;
        }else
        {
            rstr +=str.charCodeAt(i).toString(16)
        }
    }
    return rstr;
}

function Encode()
{
    var st = new Object();
    var mb = document.getElementById("mb").value;
    if (mb != undefined && mb!="")
    {
        st.letters = mb;
    }
    InitJz( st);
    //var zlx = '分离码是一种信息编解码技术，主要利用数学的不同进制转换来形成，结合码表和数学的进制转换，提出码位分离的编解码方法。';

    var zlx = document.getElementById("ss").value;
    var code = encodeURI(zlx);
    var s_str = Encode16(code);

    var co = new Array();
    var m_str = "";
    for (var i = 0; i < s_str.length/2; i++)
    {
        co.push(s_str.substr(i * 2, 2));
        m_str+="%"+s_str.substr(i * 2, 2);
    }
    var m = decodeURI(m_str);
    var iSer = new Array();
    var zxj = EncodeJz(st, co, iSer);
    document.getElementById("dd").value = zxj;
    var pp = DecodeJz(st, zxj, zxj.length, iSer);
    var msg = "";
    for (var i = 0; i < pp.length / 2; i++) {
        msg += "%" + pp.substr(i * 2, 2);
    }
    document.getElementById("dl").value = iSer.toString();
    document.getElementById("d1").value = msg;
    document.getElementById("de").value = unescape(decodeURI(msg));
}
function Decode()
{
    var st = new Object();
    var mb = document.getElementById("mb").value;
    if (mb != undefined && mb != "") {
        st.letters = mb;
    }
    InitJz(st);
    var zxj = document.getElementById("dd").value;
    var iSer = document.getElementById("dl").value.split(",");
    for (x in iSer)
    {
        iSer[x] =parseInt( iSer[x]);
    }
    var pp = DecodeJz(st, zxj, zxj.length, iSer);
    var msg = "";
    for (var i = 0; i < pp.length / 2; i++) {
        msg += "%" + pp.substr(i * 2, 2);
    }
    document.getElementById("d1").value = msg;
    try{
        document.getElementById("de").value = unescape(decodeURI(msg));
    }catch(e)
    {
        document.getElementById("de").value = e.message;
    }
    
}