import pyrebase
from pusher_push_notifications import PushNotifications

config = {
    'apiKey': "AIzaSyBKlUh1IR2KnS5P3HvIqDyu-Kdu7HIOwCI",
    'authDomain': "smarbin-ridwan.firebaseapp.com",
    'databaseURL': "https://smarbin-ridwan.firebaseio.com",
    'projectId': "smarbin-ridwan",
    'storageBucket': "smarbin-ridwan.appspot.com",
    'messagingSenderId': "814524367454",
    'appId': "1:814524367454:web:c4d30644ca646698396acf"
}

firebase = pyrebase.initialize_app(config)

db = firebase.database()
pn_client = PushNotifications(
    instance_id='b16693f8-071b-4ba1-a33c-d7cc489f2a1e',
    secret_key='2A65ACFA13FB2C772ED44A78932B282BD9A5A70131FA0386C6989990C72F8105',
)


def stream_handler(message):
    print(message)
    if (message['data'] is 2):
        response = pn_client.publish_to_interests(
            interests=['hello'],
            publish_body={
                'apns': {
                    'aps': {
                        'alert': 'Default'
                    }
                },
                'fcm': {
                    'notification': {
                        'title': 'Muatan Sampah Sudah Mulai Penuh!',
                        'body': 'Segera dibersihkan ya! Pahlawan petugas kebersihanku'
                    }
                }
            }
        )
    elif (message['data'] is 11):
        response = pn_client.publish_to_interests(
            interests=['hello'],
            publish_body={
                'apns': {
                    'aps': {
                        'alert': 'Default'
                    }
                },
                'fcm': {
                    'notification': {
                        'title': 'Tempat Sampah Sudah Hampir Setengah Muatan!',
                        'body': 'Lebih baik sih segera dibersihkan ya! biar lingkungan makin indah kaya kamu'
                    }
                }
            }
        )
    elif (message['data'] is 3):
        response = pn_client.publish_to_interests(
            interests=['hello'],
            publish_body={
                'apns': {
                    'aps': {
                        'alert': 'Default'
                    }
                },
                'fcm': {
                    'notification': {
                        'title': 'Tempat Sampah Sudah Hampir Penuh!',
                        'body': 'Mohon sangat segera dibersihkan ya! aku gamau kita jadi sakit'
                    }
                }
            }
        )
    elif (message['data'] is 4):
        response = pn_client.publish_to_interests(
            interests=['hello'],
            publish_body={
                'apns': {
                    'aps': {
                        'alert': 'Default'
                    }
                },
                'fcm': {
                        'notification': {
                            'title': 'Sampah Sudah Lama Didiamkan!',
                            'body': 'Dibersihkan detik ini juga ya! aku gamau lingkungan kita jadi tercemar'
                      }
                 }
            }
        )
    elif (message['data'] is 404):
        response = pn_client.publish_to_interests(
            interests=['hello'],
            publish_body={
                'apns': {
                    'aps': {
                        'alert': 'Default'
                    }
                },
                'fcm': {
                    'notification': {
                        'title': 'Oh tidak!',
                        'body': 'Sepertinya tempat sampahnya sedang bermasalah, segera di cek ya!'
                    }
                }
            }
        )
        print(response['publishId'])


# all_users = db.child("bins").get()
# for user in all_users.each():
#     print(user.key())

for datas in range(48):
    # get_name = db.child("bins/", datas, "/nama_bin").get()
    # print(get_name.val())
    my_stream = db.child("bins/", datas, "/bin_status").stream(stream_handler, None)
