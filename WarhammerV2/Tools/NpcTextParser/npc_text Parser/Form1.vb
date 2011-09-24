Imports System.Threading
' Imports Notification_Center.Callback

Public Class Form1

    Delegate Sub ItemsAddCallback(ByVal obj As Object, ByVal str As String)
    Delegate Sub ItemsTextCallback(ByVal obj As Object, ByVal str As String)

    Dim creatures As New List(Of String)
    Dim ignored As New List(Of String)

    Dim HTML, HTML2, HTML3 As String

    Public Sub ItemsAdd(ByVal obj As Object, ByVal [text] As String)
        If obj.InvokeRequired Then
            Dim d As New ItemsAddCallback(AddressOf ItemsAdd)
            Me.Invoke(d, New Object() {obj, [text]})
        Else
            obj.Items.Add([text])
        End If
    End Sub
    Public Sub ItemsText(ByVal obj As Object, ByVal [text] As String)
        If obj.InvokeRequired Then
            Dim d As New ItemsTextCallback(AddressOf ItemsText)
            Me.Invoke(d, New Object() {obj, [text]})
        Else
            obj.Text &= [text]
        End If
    End Sub

    Private myThread, myThread2 As Thread

    Public Function Between(ByRef src As String, ByRef start As String, ByRef ended As String, ByRef del As Boolean) As String
        Dim ret As String = ""
        Dim idxStart As Integer = src.IndexOf(start)
        If idxStart <> -1 Then
            idxStart = idxStart + Len(start)
            Dim idxEnd As Integer = src.IndexOf(ended, idxStart)
            If idxEnd <> -1 Then
                ret = src.Substring(idxStart, idxEnd - idxStart)
                If del = True Then 'Thanks to ByRef
                    src = src.Replace(start & ret & ended, vbNullString)
                End If
            End If
        End If
        Return ret
    End Function

    Private Sub ThreadOnline()
        For Each Stringed In creatures
            ItemsAdd(ListBox1, "ALLA: " & Stringed)
            Try
                Dim webClient As System.Net.WebClient = New System.Net.WebClient()
                HTML = webClient.DownloadString("http://l2.allakhazam.com/wiki/war_mob:" & Stringed)
                While Between(HTML, "Too many accesses from your", "period", False) <> ""
                    ItemsText(TextBox1, "-- Too many accesses " & Date.Now() & vbCrLf)
                    Thread.Sleep(60000)
                    HTML = webClient.DownloadString("http://l2.allakhazam.com/wiki/war_mob:" & Stringed)
                End While
                If Between(HTML, "<title>", " ::", False).Equals(Stringed) Then
                    HTML2 = HTML
                Else
                    Dim webClient2 As System.Net.WebClient = New System.Net.WebClient()
                    HTML2 = webClient2.DownloadString("http://war.allakhazam.com/db/mob.html?warmob=" & Between(HTML, "warmob=", Chr(34), False))
                    While Between(HTML2, "Too many accesses from your", "period", False) <> ""
                        ItemsText(TextBox1, "-- Too many accesses " & Date.Now() & vbCrLf)
                        Thread.Sleep(60000)
                        HTML2 = webClient2.DownloadString("http://war.allakhazam.com/db/mob.html?warmob=" & Between(HTML, "warmob=", Chr(34), False))
                    End While
                End If
                HTML2 = HTML2.Replace("<i></i>", "")
                If Between(HTML2, "<i>", "</i>", False) <> "" Then
                    Dim myCurrentText As String = ""
                    ItemsText(TextBox1, "SET @entry = '';" & vbCrLf)
                    ItemsText(TextBox1, "SET @entry = (SELECT Entry FROM creature_protos WHERE name LIKE '" & Stringed & "' LIMIT 1);" & vbCrLf)
                    Do While Between(HTML2, "<i>", "</i>", False) <> ""
                        myCurrentText = Between(HTML2, "<i>", "</i>", True)
                        If Not (myCurrentText.Contains(Stringed) Or myCurrentText.Contains("This article") Or myCurrentText.Contains("For other Bestiary")) Then
                            Do While Between(myCurrentText, "<", ">", False) <> ""
                                myCurrentText = myCurrentText.Replace(Between(myCurrentText, "<", ">", True), "")
                                myCurrentText = myCurrentText.Replace("<>", "")
                            Loop
                            myCurrentText = myCurrentText.Replace(Chr(34), "")
                            myCurrentText = myCurrentText.Replace("'", "''")
                            ItemsText(TextBox1, "INSERT INTO creature_texts VALUES (UUID(),@entry,'" & myCurrentText & "');" & vbCrLf)
                        End If
                    Loop
                End If
            Catch ex As Exception
                '    MsgBox(ex.Message)
            End Try
        Next
    End Sub
    Private Sub ThreadOnline2()
        For Each Stringed In creatures
            ItemsAdd(ListBox1, "WIKI: " & Stringed)
            Try
                Dim webClient3 As System.Net.WebClient = New System.Net.WebClient()
                HTML3 = webClient3.DownloadString("http://warhammeronline.wikia.com/wiki/" & Stringed)
                If Between(HTML3, "<title>", " -", False).Equals(Stringed) Then
                    HTML3 = HTML3.Replace("<i></i>", "")
                    If Between(HTML3, "<i>", "</i>", False) <> "" Then
                        Dim myCurrentText As String = ""
                        ItemsText(TextBox1, "SET @entry = '';" & vbCrLf)
                        ItemsText(TextBox1, "SET @entry = (SELECT Entry FROM creature_protos WHERE name LIKE '" & Stringed & "' LIMIT 1);" & vbCrLf)
                        Do While Between(HTML3, "<i>", "</i>", False) <> ""
                            myCurrentText = Between(HTML3, "<i>", "</i>", True)
                            If Not (myCurrentText.Contains(Stringed) Or myCurrentText.Contains("This article") Or myCurrentText.Contains("For other Bestiary")) Then
                                Do While Between(myCurrentText, "<", ">", False) <> ""
                                    myCurrentText = myCurrentText.Replace(Between(myCurrentText, "<", ">", True), "")
                                    myCurrentText = myCurrentText.Replace("<>", "")
                                Loop
                                myCurrentText = myCurrentText.Replace(Chr(34), "")
                                myCurrentText = myCurrentText.Replace("'", "''")
                                ItemsText(TextBox1, "INSERT INTO creature_texts VALUES (UUID(),@entry,'" & myCurrentText & "');" & vbCrLf)
                            End If
                        Loop
                    End If
                End If
            Catch ex As Exception
                '    MsgBox(ex.Message)
            End Try
        Next
    End Sub

    Private Sub Form1_Load(sender As System.Object, e As System.EventArgs) Handles MyBase.Load
        ignored.Add("For other Bestiary entries see the index.")
        ignored.Add("- ")
        Dim FILE_NAME As String = "creature_protos.txt"
        If System.IO.File.Exists("creature_protos.txt") Then
            Dim objReader As New System.IO.StreamReader(FILE_NAME)
            While Not objReader.EndOfStream
                creatures.Add(objReader.ReadLine())
            End While
            objReader.Close()
            myThread = New Thread(New ThreadStart(AddressOf ThreadOnline))
            myThread.Start()
            myThread2 = New Thread(New ThreadStart(AddressOf ThreadOnline2))
            myThread2.Start()
        End If
    End Sub

    Private Sub Form1_Close(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.FormClosed
        Dim myProcess As System.Diagnostics.Process = System.Diagnostics.Process.GetCurrentProcess()
        myProcess.Kill()
    End Sub
End Class
