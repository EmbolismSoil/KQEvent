//
// Created by lee on 16-9-12.
//

#include "TCPInfo.h"
#include <common.h>

namespace KQEvent {
    TCPInfo::TCPInfo(::tcp_info const &info) :
            tcp_info(info) {

    }

    const TCPInfo::TCPInfoPtr TCPInfo::fromTCPSocketFd(int fd)
    throw(KQEventCommonException) {
        ::socklen_t len = 0;
        ::tcp_info info;

        int ret = ::getsockopt(fd, SOL_TCP, TCP_INFO, &info, &len);
        if (ret < 0) {
            std::string err = errno2String(errno);
            throw (KQEventCommonException(err));
        }

        auto aNew = new TCPInfo(info);
        auto infoPtr = TCPInfoPtr(aNew);
        if (ret >= 0)
            infoPtr->__format();

        return infoPtr;
    }

    std::string const &TCPInfo::toString() {
        return _infoString;
    }

    void TCPInfo::__format() {
        const char *format =
                "tcpi_state : %u\n"
                        "tcpi_ca_state : %u\n"
                        "tcpi_retransmits : %u\n"
                        "tcpi_probes : %u\n"
                        "tcpi_backoff : %u\n"
                        "tcpi_options : %u\n"
                        "tcpi_snd_wscale : %u\n"
                        "tcpi_rto : %u\n"
                        "tcpi_ato : %u\n"
                        "tcpi_snd_mss : %u\n"
                        "tcpi_rcv_mss : %u\n"
                        "tcpi_unacked : %u\n"
                        "tcpi_sacked : %u\n"
                        "tcpi_lost : %u\n"
                        "tcpi_retrans : %u\n"
                        "tcpi_fackets : %u\n"
                        "tcpi_last_data_sent : %u\n"
                        ""
                        "tcpi_last_ack_sent : %u\n"
                        "tcpi_last_data_recv : %u\n"
                        "tcpi_last_ack_recv : %u\n"
                        "tcpi_pmtu : %u\n"
                        "tcpi_rcv_ssthresh : %u\n"
                        "tcpi_rtt : %u\n"
                        "tcpi_rttvar : %u\n"
                        "tcpi_snd_ssthresh : %u\n"
                        "tcpi_snd_cwnd : %u\n"
                        "tcpi_advmss : %u\n"
                        "tcpi_reordering : %u\n"
                        "tcpi_rcv_rtt : %u\n"
                        "tcpi_rcv_space : %u\n"
                        "tcpi_total_retrans : %u\n";

        char buf[1024];
        sprintf(buf, format, tcpi_state,
                tcpi_ca_state,
                tcpi_retransmits,
                tcpi_probes,
                tcpi_backoff,
                tcpi_options,
                tcpi_snd_wscale,
                tcpi_rcv_wscale,
                tcpi_rto,
                tcpi_ato,
                tcpi_snd_mss,
                tcpi_rcv_mss,
                tcpi_unacked,
                tcpi_sacked,
                tcpi_lost,
                tcpi_retrans,
                tcpi_fackets,
                tcpi_last_data_sent,
                tcpi_last_ack_sent,
                tcpi_last_data_recv,
                tcpi_last_ack_recv,
                tcpi_pmtu,
                tcpi_rcv_ssthresh,
                tcpi_rtt,
                tcpi_rttvar,
                tcpi_snd_ssthresh,
                tcpi_snd_cwnd,
                tcpi_advmss,
                tcpi_reordering,
                tcpi_rcv_rtt,
                tcpi_rcv_space,
                tcpi_total_retrans);

        _infoString = std::string(buf);

    }
}

