//
// Created by lee on 16-9-12.
//

#ifndef KQEVENT_TCPINFO_H
#define KQEVENT_TCPINFO_H

#include <sys/socket.h>
#include <sys/types.h>
#include <asm/types.h>
#include <string>
#include <memory>
#include <netinet/tcp.h>
#include "KQEventCommonException.h"

#if 0
struct tcp_info {
__u8 tcpi_state; /* TCP状态 */
__u8 tcpi_ca_state; /* TCP拥塞状态 */
__u8 tcpi_retransmits; /* 超时重传的次数 */
__u8 tcpi_probes; /* 持续定时器或保活定时器发送且未确认的段数*/
__u8 tcpi_backoff; /* 退避指数 */
__u8 tcpi_options; /* 时间戳选项、SACK选项、窗口扩大选项、ECN选项是否启用*/
__u8 tcpi_snd_wscale : 4, tcpi_rcv_wscale : 4; /* 发送、接收的窗口扩大因子*/
__u32 tcpi_rto; /* 超时时间，单位为微秒*/
__u32 tcpi_ato; /* 延时确认的估值，单位为微秒*/
__u32 tcpi_snd_mss; /* 本端的MSS */
__u32 tcpi_rcv_mss; /* 对端的MSS */

__u32 tcpi_unacked; /* 未确认的数据段数，或者current listen backlog */
__u32 tcpi_sacked; /* SACKed的数据段数，或者listen backlog set in listen() */
__u32 tcpi_lost; /* 丢失且未恢复的数据段数 */
__u32 tcpi_retrans; /* 重传且未确认的数据段数 */
__u32 tcpi_fackets; /* FACKed的数据段数 */

/* Times. 单位为毫秒 */
__u32 tcpi_last_data_sent; /* 最近一次发送数据包在多久之前 */
__u32 tcpi_last_ack_sent;  /* 不能用。Not remembered, sorry. */
__u32 tcpi_last_data_recv; /* 最近一次接收数据包在多久之前 */
__u32 tcpi_last_ack_recv; /* 最近一次接收ACK包在多久之前 */

/* Metrics. */
__u32 tcpi_pmtu; /* 最后一次更新的路径MTU */
__u32 tcpi_rcv_ssthresh; /* current window clamp，rcv_wnd的阈值 */
__u32 tcpi_rtt; /* 平滑的RTT，单位为微秒 */
__u32 tcpi_rttvar; /* 四分之一mdev，单位为微秒v */
__u32 tcpi_snd_ssthresh; /* 慢启动阈值 */
__u32 tcpi_snd_cwnd; /* 拥塞窗口 */
__u32 tcpi_advmss; /* 本端能接受的MSS上限，在建立连接时用来通告对端 */
__u32 tcpi_reordering; /* 没有丢包时，可以重新排序的数据段数 */

__u32 tcpi_rcv_rtt; /* 作为接收端，测出的RTT值，单位为微秒*/
__u32 tcpi_rcv_space;  /* 当前接收缓存的大小 */

__u32 tcpi_total_retrans; /* 本连接的总重传个数 */
};
#endif
namespace KQEvent {
    class TCPInfo : public ::tcp_info{
    public:
        using TCPInfoPtr = std::shared_ptr<TCPInfo>;

        static TCPInfoPtr const fromTCPSocketFd(int fd)
                                throw(KQEventCommonException);

        std::string const &toString();

    private:
        TCPInfo(::tcp_info const &info);
        void __format();
        std::string _infoString;
    };
}


#endif //KQEVENT_TCPINFO_H
