

http_hooks {
        on_publish_receivevideo http://10.112.179.22/api/v1/streams;
        on_publish_receiveaudio http://10.112.179.22/api/v1/streams;
        on_publish http://10.112.179.22/api/v1/streams;
        on_stop http://10.112.179.22/api/v1/sessions;
        on_unpublish http://10.112.179.22/api/v1/streams;
        on_play http://10.112.179.22/api/v1/sessions;
        enabled on;
    }

sessions：

#define URL_SRS_POST "/api/v1/streams"


1 查看生产srs配置

publishser":   "124.250.120.81:1935",  10.106.52.162

cd /usr/local/srs

virtual int http_hooks_on_play();


int SrsRtmpConn::stream_service_cycle()

start to play stream 

api-videoconfer.gomeplus.com
/usr/local/srs/objs/srs.log


void dns_down_getredis_time::get_public_server(string& clientip, string& answerip, string& key)

bool dns_down_getredis_time::reg_redis_obser



用户选择 推送和播放视频：回控服务选择选择那一台 推送和播放
void dns_down_getredis_time::get_stream_play_server(vector<string> vtPlayIP, string& clientip, string& answerip, string& key)

curl -v -X GET http://10.1.0.222:1985/api/v1/streams/
curl -v -X GET http://10.1.0.222:1985/api/v1/streams/24420
