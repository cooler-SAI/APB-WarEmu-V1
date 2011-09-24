Module Module1

    Dim filelist As New List(Of String)
    Dim i As Integer ' LOOP
    Dim total, todo, total2 As Integer
    Dim oFile As System.IO.File
    Dim oWrite As System.IO.StreamWriter

    Public Function Between(ByRef src As String, ByRef start As String, ByRef ended As String, ByRef del As Boolean) As String
        Dim ret As String = vbNullString
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

    Public Function hex2ascii(ByVal hextext As String) As String
        Dim Value As String = ""
        For y = 1 To Len(hextext)
            Dim num = Mid(hextext, y, 2)
            If Val("&h" & num) <= 122 And Val("&h" & num) >= 32 Then
                If num = "20" And (y = 1 Or y = Len(hextext) - 1) Then
                    ' SKIP
                Else
                    Value &= Chr(Val("&h" & num))
                End If

            End If
            y = y + 1
        Next y
        hex2ascii = Value.Replace("'", "''")
    End Function

    Public Function getCreature(ByVal myLine As String) As String
        Dim sqlreq As String = ""
        If myLine.Contains("F_CREATE_MONSTER") Then
            i = 0
            todo += 1
            Try
                myLine = myLine.Replace(Between(myLine, "[", "]", True), "")
                myLine = myLine.Replace("[]", "")
                Dim myBytes = myLine.Split(" ")
                While i < myBytes.Length - 1
                    Dim oid = myBytes(0) & myBytes(1)
                    ' SKIP 2 3
                    Dim worldO = Convert.ToInt32(myBytes(4) & myBytes(5), 16)
                    Dim worldZ = Convert.ToInt32(myBytes(6) & myBytes(7), 16)
                    Dim worldX = Convert.ToInt64(myBytes(8) & myBytes(9) & myBytes(10) & myBytes(11), 16)
                    Dim worldY = Convert.ToInt64(myBytes(12) & myBytes(13) & myBytes(14) & myBytes(15), 16)
                    ' SKIP 16 17
                    Dim displayid = Convert.ToInt32(myBytes(18) & myBytes(19), 16)
                    Dim scale = Convert.ToInt32(myBytes(20), 16)
                    Dim level = Convert.ToInt32(myBytes(21), 16)
                    Dim faction = Convert.ToInt32(myBytes(22), 16)
                    ' SKIP 23 24 25 26
                    Dim emote = Convert.ToInt32(myBytes(27), 16)
                    ' SKIP 28 29
                    Dim unk1 = myBytes(30)
                    ' SKIP 31
                    Dim unk2 = Convert.ToInt32(myBytes(32) & myBytes(33), 16)
                    Dim unk3 = Convert.ToInt32(myBytes(34) & myBytes(35), 16)
                    Dim unk4 = Convert.ToInt32(myBytes(36) & myBytes(37), 16)
                    Dim unk5 = Convert.ToInt32(myBytes(38) & myBytes(39), 16)
                    Dim unk6 = Convert.ToInt32(myBytes(40) & myBytes(41), 16)
                    Dim title = Convert.ToInt32(myBytes(42) & myBytes(43), 16)
                    Dim bytes_count = Integer.Parse(myBytes(44))
                    Dim bytes As New List(Of String)
                    Dim bytesSTR = ""
                    For i = 45 To 44 + bytes_count
                        bytes.Add(myBytes(i))
                        bytesSTR &= Convert.ToInt32(myBytes(i), 16) & ";"
                    Next
                    ' SKIP
                    i += 1
                    Dim name As String = ""
                    Do While Not myBytes(i).Equals("5E")
                        name &= myBytes(i)
                        i += 1
                    Loop
                    name = hex2ascii(name)
                    ' Console.WriteLine(name)
                    ' SKIP taille variable
                    Do While Not (myBytes(i).Equals("01") And myBytes(i + 1).Equals("0A"))
                        i += 1
                    Loop
                    ' SKIP
                    i += 5
                    Dim ico = Convert.ToInt32(myBytes(i), 16)
                    Dim unk7 = Convert.ToInt32(myBytes(i + 1), 16)
                    ' SKIP
                    i += 3
                    Dim flags = Convert.ToInt64(myBytes(i) & myBytes(i + 1) & myBytes(i + 2) & myBytes(i + 3) & myBytes(i + 4) & myBytes(i + 5) & myBytes(i + 6) & myBytes(i + 7), 16)
                    Dim life = Convert.ToInt32(myBytes(i + 8), 16)
                    ' SKIP
                    i += 10
                    Dim zoneid = Convert.ToInt32(myBytes(i), 16)
                    oWrite.WriteLine("SET @entry = (SELECT Entry FROM creature_protos WHERE name LIKE '" & name & "' LIMIT 1);")
                    oWrite.WriteLine("INSERT IGNORE INTO creature_spawns VALUES ('',@entry,'" & zoneid & "','" & worldX & "','" & worldY & "','" & worldZ & "','" & worldO & "','" & bytesSTR & "','" & ico & "','" & emote & "','" & title & "','');")
                    ' SKIP
                    i += 9
                    ' DEDUCTION
                    Do While i + 3 < myBytes.Length - 1 AndAlso Not ((Convert.ToInt32(myBytes(i), 16) <= 2) And (Convert.ToInt32(myBytes(i + 1), 16) <= 15) And (Convert.ToInt32(myBytes(i + 2), 16) <= 1) And (Convert.ToInt32(myBytes(i + 3), 16) <= 1))
                        i += 1
                    Loop
                    i += 1
                    Dim item_count = Convert.ToInt32(myBytes(i), 16)
                    ' MOVE CURSOR
                    i += 1
                    ' METHODE 2 : DEDUCTION
                    If item_count < 15 And item_count > 0 Then
                        total2 += 1
                        ' FORM1: 00 00 0A 06 50
                        ' FORM2: 01 00 14 0C 1D
                        While i + 2 < myBytes.Length - 1
                            While Not ((Convert.ToInt32(myBytes(i), 16) <= 1) And (Convert.ToInt32(myBytes(i + 1), 16) = 0) And (Convert.ToInt32(myBytes(i + 2), 16) <= 30) And (Convert.ToInt32(myBytes(i + 2), 16) >= 10))
                                i += 1
                            End While
                            Dim slot = myBytes(i + 1) & myBytes(i + 2)
                            Dim item = myBytes(i + 3) & myBytes(i + 4)
                            oWrite.WriteLine("INSERT IGNORE INTO creature_items VALUES (@entry,'" & Convert.ToInt32(slot, 16) & "','" & Convert.ToInt32(item, 16) & "','','');")
                            i += 4
                        End While
                    End If

                    ' METHODE 1 : REVERSE ENGINEERING
                    'If item_count < 15 And item_count > 0 Then
                    '    Dim k As Integer = 0
                    '    For j = i To myBytes.Length - 4
                    '        If k < item_count Then
                    '            Dim effect = Integer.Parse(myBytes(j))
                    '            Dim slot = myBytes(j + 1) & myBytes(j + 2)
                    '            Dim item = myBytes(j + 3) & myBytes(j + 4)
                    '            Dim effect_byte As String = 0
                    '            If effect > 0 Then
                    '                effect_byte = myBytes(j + 5) & myBytes(j + 6) & myBytes(j + 7) & myBytes(j + 8)
                    '                j += 8
                    '            Else
                    '                j += 4
                    '            End If
                    '            k += 1
                    '            'Console.WriteLine("slot: " & slot & vbCrLf & "item: " & item & vbCrLf & "effect: " & effect_byte)
                    '            oWrite.WriteLine("INSERT IGNORE INTO creature_items VALUES (@entry,'" & Convert.ToInt32(slot, 16) & "','" & Convert.ToInt32(item, 16) & "','" & Convert.ToInt32(effect_byte, 16) & "','');")
                    '        End If
                    '    Next
                    '    total2 += 1
                    'End If
                    i = myBytes.Length - 1
                End While
            Catch e As Exception
                'Console.WriteLine("Despite all our efforts, an error occured.")
            End Try
            total += 1
        End If
        Return sqlreq
    End Function

    Sub Main()
        total = 0
        total2 = 0
        todo = 0
        Console.WriteLine("")
        Console.WriteLine("                             WARHAMMER ONLINE")
        Console.WriteLine("                             AGE OF RECKONING")
        Console.WriteLine("")
        Console.WriteLine("")
        Console.WriteLine("                         F_CREATE_MONSTER PARSER")
        Console.WriteLine("                               by Valkirie")
        Console.WriteLine("")
        Console.WriteLine("")
        Console.WriteLine("                           Press ENTER to start")
        Console.ReadKey()
        Console.WriteLine("                             Process started")
        oWrite = IO.File.CreateText("out.sql")
        For steps = 0 To 100
            Dim FILE_NAME As String = "sniff" & steps & ".txt"
            If System.IO.File.Exists(FILE_NAME) Then
                Console.WriteLine("")
                Console.WriteLine("")
                Console.WriteLine("Processing ... " & FILE_NAME)
                Dim objReader As New System.IO.StreamReader(FILE_NAME)
                While Not objReader.EndOfStream
                    getCreature(objReader.ReadLine())
                End While
                objReader.Close()
                Console.WriteLine(total & " creatures have been more or less converted from a total of " & todo & ".")
                Console.WriteLine("Among which " & total2 & " has equipments.")
            End If
        Next
        oWrite.Close()
        Console.ReadKey()
    End Sub

End Module