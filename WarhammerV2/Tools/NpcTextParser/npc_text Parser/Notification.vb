Imports System.Net
Imports System.Text
Imports System.IO

Public Class Notification
    Private uri, text1, text2 As String

    Public Sub New(str1 As String, str2 As String, id As String)
        text1 = str1
        text2 = str2
        uri = id
    End Sub

    Public Function sendToast() As String
        Try
            Dim sendNotificationRequest As HttpWebRequest = DirectCast(WebRequest.Create(uri), HttpWebRequest)
            sendNotificationRequest.Method = "POST"
            Dim toastMessage As String = "<?xml version=""1.0"" encoding=""utf-8""?>" & "<wp:Notification xmlns:wp=""WPNotification"">" & "<wp:Toast>" & "<wp:Text1>" & text1 & "</wp:Text1>" & "<wp:Text2>" & text2 & "</wp:Text2>" & "</wp:Toast> " & "</wp:Notification>"
            Dim notificationMessage As Byte() = Encoding.[Default].GetBytes(toastMessage)
            sendNotificationRequest.ContentLength = notificationMessage.Length
            sendNotificationRequest.ContentType = "text/xml"
            sendNotificationRequest.Headers.Add("X-WindowsPhone-Target", "toast")
            sendNotificationRequest.Headers.Add("X-NotificationClass", "2")
            Using requestStream As Stream = sendNotificationRequest.GetRequestStream()
                requestStream.Write(notificationMessage, 0, notificationMessage.Length)
            End Using
            Dim response As HttpWebResponse = DirectCast(sendNotificationRequest.GetResponse(), HttpWebResponse)
            Dim notificationStatus As String = response.Headers("X-NotificationStatus")
            Dim notificationChannelStatus As String = response.Headers("X-SubscriptionStatus")
            Dim deviceConnectionStatus As String = response.Headers("X-DeviceConnectionStatus")
            Return "Notification Status: " & notificationStatus & vbCrLf & "Subscription Status: " & notificationChannelStatus & vbCrLf & "Connection Status: " & deviceConnectionStatus
        Catch ex As Exception
            MsgBox("Exception caught sending update: " & ex.ToString())
        End Try
        Return vbEmpty
    End Function
End Class