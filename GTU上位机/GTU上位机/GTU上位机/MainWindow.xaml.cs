using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;
using System.IO;
using System.Windows.Threading;

namespace GTU上位机
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public System.Windows.Forms.OpenFileDialog openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
        public System.Windows.Threading.DispatcherTimer dTimer = new DispatcherTimer();
        public SerialPort sp = new SerialPort();
        public Byte[] CONNECTTO = { 0xf5, 0xfa, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };
        bool portpoen = false;
        bool connect = false;
        byte[] crc = new byte[2];
        public MainWindow()
        {
            //string nametmp ="";
            InitializeComponent();
            ADCset_button.IsEnabled = false;
            ADCget_button.IsEnabled = false;
            Time_button.IsEnabled = false;
            Clear_button.IsEnabled = false;
            Check_button.IsEnabled = false;
            start_button.IsEnabled = false;
            sp.BaudRate = 9600;
            open_button.Content = "打开";
            start_button.Content = "连接";
            checkcom();
            dTimer.IsEnabled = true;
            dTimer.Interval = TimeSpan.FromMilliseconds(1000);
            dTimer.Tick += new EventHandler(dTimer_Tick);
            dTimer.Start();
        }

        private void open_button_Click(object sender, RoutedEventArgs e)
        {
            if (false == portpoen)
            {
                if (("" == comnum.Text))
                {
                    MessageBox.Show("选择错误");
                    return;
                }
                sp.PortName = comnum.Text;
                sp.BaudRate = 115200; /*波特率固定*/
                sp.Open();
                if (false == sp.IsOpen)
                {
                    MessageBox.Show("打开错误");
                    checkcom();
                    return;
                }
                else
                {
                    start_button.IsEnabled = true;
                    sp.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(sp_DataReceived);
                    portpoen = true;
                    open_button.Content = "关闭";

                }
            }
            else
            {
                
                try
                {
                    sp.Close();
                    open_button.Content = "打开";
                    portpoen = false;
                    checkcom();
                }
                catch (Exception)
                {
                    MessageBox.Show("关闭错误");
                    throw;
                }
            }
        }

        private void checkcom()
        {
            int i = 0;
            string[] ports;//可用串口数组
            comnum.Items.Clear();
            ports = SerialPort.GetPortNames();
            sp.BaudRate = 115200;
            for (i = 0; i < ports.Length; i++)
            {
                sp.PortName = ports[i];
                try
                {
                    sp.Open();
                }
                catch
                {
                    continue;
                    throw;
                }
                sp.Close();
                comnum.Items.Add(ports[i]);//下拉控件里添加可用串口
            }
            ADCset_button.IsEnabled = false;
            ADCget_button.IsEnabled = false;
            Time_button.IsEnabled = false;
            Clear_button.IsEnabled = false;
            Check_button.IsEnabled = false;
            start_button.IsEnabled = false;
            open_button.Content = "打开";
            open_button.IsEnabled = true;
            start_button.Content = "连接";
        }

        private void dTimer_Tick(Object myObject, EventArgs myEventArgs)
        {
            textBlock.Text = DateTime.Now.ToLongDateString() + DateTime.Now.ToLongTimeString();
           // textBlock.Text = DateTime.Now.ToLongDateString();

        }
        private void sp_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            int num = sp.BytesToRead;
            byte tmp = 0;
            byte[] tmpbuf = new byte[num+20];
            if(-1 == num)
            {
                return;
            }
            try
            {
                sp.Read(tmpbuf, 0, num);
            }
            catch (Exception)
            {
                MessageBox.Show("接收错误");
                try
                {
                    sp.Close();
                    MessageBox.Show("串口关闭，请重新打开");
                    open_button.Content = "打开";
                    open_button.IsEnabled = true;
                    comnum.IsEnabled = true;
                    portpoen = false;
                    connect = false;
                    start_button.Content = "连接";
                    checkcom();
                }
                catch (Exception)
                {
                    throw;
                }
//                throw;
            }
            if(0xF5 == tmpbuf[0] && 0xFA == tmpbuf[1]) /*帧头正确*/
            {
                GetCRC(tmpbuf, num, ref crc);
                if(0 == crc[0] && 0 == crc[1]) /*crc校验正确*/
                {
                    switch (tmpbuf[3])
                    {
                        case 0x81:
                            this.Dispatcher.Invoke(new Action(() =>
                            {
                                ADCset_button.IsEnabled = true;
                                ADCget_button.IsEnabled = true;
                                Time_button.IsEnabled = true;
                                Clear_button.IsEnabled = true;
                                Check_button.IsEnabled = true;
                                start_button.Content = "断开";
                                comnum.Text = sp.PortName;
                                comnum.IsEnabled = false;
                                open_button.IsEnabled = false;
                            }));

                            connect = true; break; /*连接正常*/
                        case 0x82: MessageBox.Show("校准成功");
                            this.Dispatcher.Invoke(new Action(() =>
                            {
                                if (true == showButton.IsChecked)
                                {
                                    showButton.IsChecked = false;
                                }
                                else
                                {
                                    showButton.IsChecked = true;
                                }
                            }));
                            break;
                        case 0x83:MessageBox.Show("校时成功"); break;
                        case 0x84:MessageBox.Show("清除成功"); break;
                        case 0x85:if(0xAA == tmpbuf[5] ||0xAA ==tmpbuf[6])
                                    {
                                MessageBox.Show("校验成功");
                            }
                            else
                            {
                                MessageBox.Show("校验失败");
                            }
                            break;
                        case 0x86:
                            this.Dispatcher.Invoke(new Action(() =>
                            {
                                getvalue3.Text = Convert.ToString((tmpbuf[5] * 256 + tmpbuf[6]) / 1000.0) + "V";
                                getvalue1.Text = Convert.ToString((tmpbuf[7] * 256 + tmpbuf[8]) / 100.0) + "mA";
                                getvalue2.Text = Convert.ToString((tmpbuf[9] * 256 + tmpbuf[10]) / 100.0) + "mA";
                            })); 
                                   break;
                        case 0x87:MessageBox.Show("连接断开");
                            this.Dispatcher.Invoke(new Action(() =>
                            {
                                ADCset_button.IsEnabled = false;
                                ADCget_button.IsEnabled = false;
                                Time_button.IsEnabled = false;
                                Clear_button.IsEnabled = false;
                                Check_button.IsEnabled = false;
                                start_button.Content = "连接";
                                comnum.IsEnabled = true;
                                open_button.IsEnabled = true;
                            }));
                            
                            connect = false; break;

                        default:
                            break;
                    }
                }
            }
        }

        private void start_button_Click(object sender, RoutedEventArgs e)
        {
            byte[] tmp = new byte[20];
            if (false == connect)
            {
                tmp[0] = 0xF5;
                tmp[1] = 0xFA; /*帧头*/
                tmp[2] = 0x0A; /*数据域长度*/
                tmp[3] = 0x01; /*命令码*/
                tmp[4] = 0x00; /*地址码*/
                tmp[5] = 0x12; /*数据*/
                tmp[6] = 0x34;
                tmp[7] = 0x56;
                tmp[8] = 0x78;
                tmp[9] = tmp[10] = tmp[11] = tmp[12] = 0x00;
                GetCRC(tmp,13,ref crc);
                tmp[13] = crc[0];
                tmp[14] = crc[1];
                try
                {
                    sp.Write(tmp, 0, 15);
                }
                catch (Exception)
                {
                    MessageBox.Show("发送错误");
                    try
                    {
                        sp.Close();
                        MessageBox.Show("串口关闭，请重新打开");
                        open_button.Content = "打开";
                        open_button.IsEnabled = true;
                        comnum.IsEnabled = true;
                        portpoen = false;
                        connect = false;
                        start_button.Content = "连接";
                        checkcom();
                    }
                    catch (Exception)
                    {
                        throw;
                    }
//                    throw;
                }
                
            }
            else
            {
                tmp[0] = 0xF5;
                tmp[1] = 0xFA; /*帧头*/
                tmp[2] = 0x0A; /*数据域长度*/
                tmp[3] = 0x07; /*命令码*/
                tmp[4] = 0x00; /*地址码*/
                tmp[5] = 0xFE; /*数据*/
                tmp[6] = 0xDC;
                tmp[7] = 0xBA;
                tmp[8] = 0x98;
                tmp[9] = tmp[10] = tmp[11] = tmp[12] = 0x00;
                GetCRC(tmp, 13, ref crc);
                tmp[13] = crc[0];
                tmp[14] = crc[1];
                try
                {
                    sp.Write(tmp, 0, 15);
                }
                catch (Exception)
                {
                    MessageBox.Show("发送错误");
                    try
                    {
                        sp.Close();
                        MessageBox.Show("串口关闭，请重新打开");
                        open_button.Content = "打开";
                        open_button.IsEnabled = true;
                        comnum.IsEnabled = true;
                        portpoen = false;
                        checkcom();
                    }
                    catch (Exception)
                    {
                        throw;
                    }
//                    throw;
                }
            }
        }
        private void GetCRC(byte[] command,int length, ref byte[] CRC)
        {
            ushort CrcTmp = 0x0; /*初值为0*/
            int i = 0, j = 0;
            ushort s = 0;
            ushort tmpcrc = 0;
            for (i = 0; i < length; i++)
            {
                tmpcrc = (ushort)(command[i] << 8);
                tmpcrc &= 0xff00;
                CrcTmp ^= tmpcrc;            /*高8位异或*/
                for (j = 0; j < 8; j++)
                {
                    s = (ushort)(CrcTmp & 0x8000);
                    CrcTmp <<= 1;
                    CrcTmp &= 0xfffe;
                    if (0 != s)
                    {
                        CrcTmp ^= 0x1021;
                    }
                }
            }

            CRC[0] = (byte)((CrcTmp >> 8) & 0xff);
            CRC[1] = (byte)(CrcTmp & 0xff);
        }

        private void setvalue_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (setvalue.Text.Length == 1 && setvalue.Text == "-")
                e.Handled = true;
            else
            {
                //屏蔽中文输入和非法字符粘贴输入  
                TextBox textBox = sender as TextBox;
                TextChange[] change = new TextChange[e.Changes.Count];
                e.Changes.CopyTo(change, 0);

                int offset = change[0].Offset;
                if (change[0].AddedLength > 0)
                {
                    double num = 0;
                    if (!Double.TryParse(textBox.Text, out num))
                    {
                        textBox.Text = textBox.Text.Remove(offset, change[0].AddedLength);
                        textBox.Select(offset, 0);
                    }
                }
            }
        }

        private void setvalue_KeyDown(object sender, KeyEventArgs e)
        {
            TextBox txt = sender as TextBox;

            //屏蔽非法按键  
            if ((e.Key >= Key.NumPad0 && e.Key <= Key.NumPad9) || e.Key == Key.Decimal || e.Key.ToString() == "Tab" || e.Key == Key.Subtract)
            {
                if ((txt.Text.Contains(".") && e.Key == Key.Decimal) || (txt.Text.Contains("-") && e.Key == Key.Subtract))
                {
                    e.Handled = true;
                    return;
                }
                e.Handled = false;
            }
            else if (((e.Key >= Key.D0 && e.Key <= Key.D9) || e.Key == Key.OemPeriod || e.Key == Key.OemMinus) && e.KeyboardDevice.Modifiers != ModifierKeys.Shift)
            {
                if ((txt.Text.Contains(".") && e.Key == Key.OemPeriod) || (txt.Text.Contains("-") && e.Key == Key.OemMinus))
                {
                    e.Handled = true;
                    return;
                }
                e.Handled = false;
            }
            else
            {
                e.Handled = true;
            }
        }

        private void ADCset_button_Click(object sender, RoutedEventArgs e)
        {
            int tmpvalue = 0;
            double tmpset = 0;
            byte[] tmpsend = new byte[20];
            if ("" == setvalue.Text)
            {
                MessageBox.Show("请输入测量真实值");
                return;
            }
            tmpset = Convert.ToDouble(setvalue.Text);
            if ("" == ADClist.Text)
            {
                MessageBox.Show("请选择ADC校准路数");
                return;
            }
            tmpsend[0] = 0xF5;
            tmpsend[1] = 0xFA; /*帧头*/
            tmpsend[2] = 0x0A; /*帧长*/
            tmpsend[3] = 0x02; /*命令码*/
            tmpsend[4] = 0x00; /*地址码*/
            if ("1" == ADClist.Text || "2" == ADClist.Text)
            {
                try
                {
                    tmpvalue = Convert.ToInt32(tmpset * 100);  /*第1、2路ADC测量4-20mA电流，精确到小数点后两位*/
                }
                catch (Exception)
                {
                    MessageBox.Show("输入有错误");
                    throw;
                }
                
                if("1" == ADClist.Text)
                {
                    tmpsend[5] = 0x02;
                }
                else
                {
                    tmpsend[5] = 0x03;
                }
            }
            else if ("3" == ADClist.Text)
            {
                tmpvalue = Convert.ToInt32(tmpset * 1000); /*电源电压精确到小数点后3位*/
                tmpsend[5] = 0x01;
            }
            if(false == showButton.IsChecked)
            {
                tmpsend[6] = 0x11;   /*第一个校准点*/
            }
            else
            {
                tmpsend[6] = 0x22; /*第二个校准点*/
            }
            if(0 <= tmpvalue && 65536 > tmpvalue) /*输入值正确*/
            {
                tmpsend[7] = Convert.ToByte((tmpvalue >> 8) & 0xFF);
                tmpsend[8] = Convert.ToByte(tmpvalue & 0xFF);          /*测量真实值*/
            }
            else
            {
                MessageBox.Show("输入数据错误");
                return;
            }
            tmpsend[9] = tmpsend[10] = tmpsend[11] = tmpsend[12] = 0x00; /*预留*/
            GetCRC(tmpsend,13,ref crc);
            tmpsend[13] = crc[0];
            tmpsend[14] = crc[1];
            try
            {
                sp.Write(tmpsend, 0, 15);
            }
            catch (Exception)
            {
                MessageBox.Show("发送错误");
                try
                {
                    sp.Close();
                    MessageBox.Show("串口关闭，请重新打开");
                    open_button.Content = "打开";
                    open_button.IsEnabled = true;
                    comnum.IsEnabled = true;
                    portpoen = false;
                    connect = false;
                    start_button.Content = "连接";
                    checkcom();
                }
                catch (Exception)
                {
                    throw;
                }
 //               throw;
            }
            
        }

        private void ADCget_button_Click(object sender, RoutedEventArgs e) /*获取实时测量数值*/
        {
            byte[] tmpsend = new byte[20];
            tmpsend[0] = 0xF5;
            tmpsend[1] = 0xFA; /*帧头*/
            tmpsend[2] = 0x0A; /*帧长*/
            tmpsend[3] = 0x06; /*命令码*/
            tmpsend[4] = 0x00; /*地址码*/
            tmpsend[5] = 0x87;
            tmpsend[6] = 0x65;
            tmpsend[7] = 0x43;
            tmpsend[8] = 0x21;
            tmpsend[9] = tmpsend[10] = tmpsend[11] = tmpsend[12] = 0;
            GetCRC(tmpsend, 13, ref crc);
            tmpsend[13] = crc[0];
            tmpsend[14] = crc[1];
            try
            {
                sp.Write(tmpsend, 0, 15);
            }
            catch (Exception)
            {
                MessageBox.Show("发送错误");
                try
                {
                    sp.Close();
                    MessageBox.Show("串口关闭，请重新打开");
                    open_button.Content = "打开";
                    open_button.IsEnabled = true;
                    comnum.IsEnabled = true;
                    portpoen = false;
                    connect = false;
                    start_button.Content = "连接";
                    checkcom();
                }
                catch (Exception)
                {
                    throw;
                }
//                throw;
            }
        }

        private void Time_button_Click(object sender, RoutedEventArgs e)
        {
            byte[] tmpsend = new byte[20];
            int tmpvalue = 0;
            tmpsend[0] = 0xF5;
            tmpsend[1] = 0xFA; /*帧头*/
            tmpsend[2] = 0x0A; /*帧长*/
            tmpsend[3] = 0x03; /*命令码*/
            tmpsend[4] = 0x00; /*地址码*/
            tmpvalue = DateTime.Now.Year;
            tmpvalue %= 100; /*只取后两位*/
            tmpvalue = (tmpvalue / 10) * 16 + tmpvalue % 10; /*转换格式*/
            tmpsend[5] = Convert.ToByte(tmpvalue & 0xFF);
            tmpvalue = DateTime.Now.Month;
            tmpvalue = (tmpvalue / 10) * 16 + tmpvalue % 10;
            tmpsend[6] = Convert.ToByte(tmpvalue & 0xFF);
            tmpvalue = DateTime.Now.Day;
            tmpvalue = (tmpvalue / 10) * 16 + tmpvalue % 10;
            tmpsend[7] = Convert.ToByte(tmpvalue & 0xFF);
            tmpvalue = DateTime.Now.Hour;
            tmpvalue = (tmpvalue / 10) * 16 + tmpvalue % 10;
            tmpsend[8] = Convert.ToByte(tmpvalue & 0xFF);
            tmpvalue = DateTime.Now.Minute;
            tmpvalue = (tmpvalue / 10) * 16 + tmpvalue % 10;
            tmpsend[9] = Convert.ToByte(tmpvalue & 0xFF);
            tmpvalue = DateTime.Now.Second;
            tmpvalue = (tmpvalue / 10) * 16 + tmpvalue % 10;
            tmpsend[10] = Convert.ToByte(tmpvalue & 0xFF);
            tmpsend[11] = tmpsend[12] = 0;
            GetCRC(tmpsend, 13, ref crc);
            tmpsend[13] = crc[0];
            tmpsend[14] = crc[1];
            try
            {
                sp.Write(tmpsend, 0, 15);
            }
            catch (Exception)
            {
                MessageBox.Show("发送错误");
                try
                {
                    sp.Close();
                    MessageBox.Show("串口关闭，请重新打开");
                    open_button.Content = "打开";
                    open_button.IsEnabled = true;
                    comnum.IsEnabled = true;
                    portpoen = false;
                    connect = false;
                    start_button.Content = "连接";
                    checkcom();
                }
                catch (Exception)
                {
                    throw;
                }
//                throw;
            }
        }

        private void Clear_button_Click(object sender, RoutedEventArgs e) /*清除记录*/
        {
            byte[] tmpsend = new byte[20];
            tmpsend[0] = 0xF5;
            tmpsend[1] = 0xFA; /*帧头*/
            tmpsend[2] = 0x0A; /*帧长*/
            tmpsend[3] = 0x04; /*命令码*/
            tmpsend[4] = 0x00; /*地址码*/
            tmpsend[5] = 0xEE;
            tmpsend[6] = 0xCC;
            tmpsend[7] = 0xEE;
            tmpsend[8] = 0xCC;
            tmpsend[9] = tmpsend[10] = tmpsend[11] = tmpsend[12] = 0;
            GetCRC(tmpsend, 13, ref crc);
            tmpsend[13] = crc[0];
            tmpsend[14] = crc[1];
            try
            {
                sp.Write(tmpsend, 0, 15);
            }
            catch (Exception)
            {
                MessageBox.Show("发送错误");
                try
                {
                    sp.Close();
                    MessageBox.Show("串口关闭，请重新打开");
                    open_button.Content = "打开";
                    open_button.IsEnabled = true;
                    comnum.IsEnabled = true;
                    portpoen = false;
                    connect = false;
                    start_button.Content = "连接";
                    checkcom();
                }
                catch (Exception)
                {
                    throw;
                }
//                throw;
            }
        }

        private void Check_button_Click(object sender, RoutedEventArgs e)
        {
            MessageBoxResult confirmToDel = MessageBox.Show("确认要清除铁电吗？", "提示", MessageBoxButton.YesNo, MessageBoxImage.Question);
            if (confirmToDel == MessageBoxResult.Yes)
            {
                byte[] tmpsend = new byte[20];
                tmpsend[0] = 0xF5;
                tmpsend[1] = 0xFA; /*帧头*/
                tmpsend[2] = 0x0A; /*帧长*/
                tmpsend[3] = 0x05; /*命令码*/
                tmpsend[4] = 0x00; /*地址码*/
                tmpsend[5] = 0xAA;
                tmpsend[6] = 0x55;
                tmpsend[7] = 0xAA;
                tmpsend[8] = 0x55;
                tmpsend[9] = tmpsend[10] = tmpsend[11] = tmpsend[12] = 0;
                GetCRC(tmpsend, 13, ref crc);
                tmpsend[13] = crc[0];
                tmpsend[14] = crc[1];
                try
                {
                    sp.Write(tmpsend, 0, 15);
                }
                catch (Exception)
                {
                    MessageBox.Show("发送错误");
                    try
                    {
                        sp.Close();
                        MessageBox.Show("串口关闭，请重新打开");
                        open_button.Content = "打开";
                        open_button.IsEnabled = true;
                        comnum.IsEnabled = true;
                        portpoen = false;
                        connect = false;
                        start_button.Content = "连接";
                        checkcom();
                    }
                    catch (Exception)
                    {
                        throw;
                    }
//                    throw;
                }
            }
            else
            {
                return;
            }
        }

    }
}
