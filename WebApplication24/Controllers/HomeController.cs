using WebApplication24.Models;
using Microsoft.AspNetCore.Mvc;
using System.Diagnostics;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Text.Json;
using System.Drawing;
using System.Drawing.Imaging; // List sınıfını kullanmak için eklenmiştir
using System.IO;
using WebApplication24.Models;

namespace WebApplication24.Controllers
{
    public class HomeController : Controller
    {
        private static TcpClient _client;
        private static NetworkStream _stream;
        private static byte[] _buffer = new byte[1024];
        private static readonly object _lock = new object();

        public async Task<IActionResult> Index()
        {
            // Ana sayfa görünümünü yükler

            return View(new MessageModel());
        }



        [HttpPost]
        public async Task<IActionResult> SendMessage(MessageModel model)
        {
            if (!EnsureConnected())
            {
                model.ReceivedMessage = "Bağlantı kurulamadı.";
                return View("Index", model);
            }

            if (!string.IsNullOrEmpty(model.Message))
            {
                byte[] data = Encoding.ASCII.GetBytes(model.Message);
                await _stream.WriteAsync(data, 0, data.Length);
            }

            int bytesRead = await _stream.ReadAsync(_buffer, 0, _buffer.Length);
            model.ReceivedMessage = Encoding.ASCII.GetString(_buffer, 0, bytesRead);

            return View("Index", model);
        }




        [HttpGet]
        public async Task<IActionResult> GetSensorData()
        {
            var model = new MessageModel();

            //try
            //{
                // Bağlantı kontrolü
                if (!EnsureConnected())
                {
                   // SetErrorMessageForModel(model, "Bağlantı kurulamadı.");
                    return Json(model);
                }

                // Sensöre veri isteği gönder
                await _stream.WriteAsync(Encoding.ASCII.GetBytes("GET_SENSOR_DATA"), 0, "GET_SENSOR_DATA".Length);

                // Sensör verisini okuyalım
                byte[] sensorBuffer = new byte[25600];  // Sensör verisi için buffer
                int sensorBytesRead = await _stream.ReadAsync(sensorBuffer, 0, sensorBuffer.Length);

                if (sensorBytesRead > 0)
                {
                    // Sensör verisini ASCII formatında string'e çevir
                    string sensorData = Encoding.ASCII.GetString(sensorBuffer, 0, sensorBytesRead);
                    ParseSensorData(sensorData, model);
                }
                else
                {
                    SetErrorMessageForModel(model, "Sensör verisi okunamadı.");
                    return Json(model);
                }

                // PGM verisini okuyalım
                byte[] pgmBuffer = new byte[25600 * 25600];  // 20 MB buffer, PGM dosyası için
                int pgmBytesRead = await _stream.ReadAsync(pgmBuffer, 0, pgmBuffer.Length);

                if (pgmBytesRead > 0)
                {
                    // PGM dosyasını kaydet
                    string pgmFilePath = Path.Combine(Directory.GetCurrentDirectory(), "wwwroot/image", "map.pgm");
                    await System.IO.File.WriteAllBytesAsync(pgmFilePath, pgmBuffer.Take(pgmBytesRead).ToArray());

                    // PGM dosyasını PNG formatına dönüştür
                    string pngFilePath = Path.Combine(Directory.GetCurrentDirectory(), "wwwroot/image", "map.png");
                    ConvertPgmToPng(pgmFilePath, pngFilePath);

                    // Model içerisine PGM ve sensör verilerini ekleyin
                    model.ImagePath = "/image/map.png";
                }
                else
                {
                    SetErrorMessageForModel(model, "PGM verisi okunamadı.");
                    return Json(model);
                }

                // Sonuçları JSON formatında döndür
                return Json(model);
            //}

            //catch (Exception ex)
            {
                // Hata oluşursa, model içerisine hata mesajını ekleyelim
                // SetErrorMessageForModel(model, $"Bir hata oluştu: {ex.Message}");
                //return Json(model);
            }
        }



        private bool EnsureConnected()
        {
            lock (_lock)
            {
                if (_client == null || !_client.Connected)
                {
                    try
                    {
                        _client = new TcpClient("192.168.235.210", 65432);
                        _stream = _client.GetStream();
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("Bağlantı hatası: " + e.Message);
                        return false;
                    }
                }
            }
            return true;
        }



        private void SetErrorMessageForModel(MessageModel model, string message)
        {
            model.ReceivedMessage = message;
            model.close_button_state = message;
            model.start_button_state = message;
            model.left_distance = message;
            model.right_distance = message;
            model.first_temperature = message;
            model.second_temperature = message;
            model.qr_code = message;
            model.current = message;
            model.emergency_stop_off = message;
            model.emergency_stop_on = message;
            model.voltage = message;
        }

        

        private void ParseSensorData(string data, MessageModel model)
        {
            var dataParts = data.Split(',');
            foreach (var part in dataParts)
            {
                var keyValue = part.Split(':');
                if (keyValue.Length == 2)
                {
                    switch (keyValue[0])
                    {
                        case "first_temperature":
                            model.first_temperature = keyValue[1];
                            break;
                        case "second_temperature":
                            model.second_temperature = keyValue[1];
                            break;
                        case "left_distance":
                            model.left_distance = keyValue[1];
                            break;
                        case "right_distance":
                            model.right_distance = keyValue[1];
                            break;
                        case "close_button_state":
                            model.close_button_state = keyValue[1];
                            break;
                        case "start_button_state":
                            model.start_button_state = keyValue[1];
                            break;
                        case "qr_code":
                            model.qr_code = keyValue[1];
                            break;
                        case "current":
                            model.current = keyValue[1];
                            break;
                        case "emergency_stop_off":
                            model.emergency_stop_off = keyValue[1];
                            break;
                        case "emergency_stop_on":
                            model.emergency_stop_on = keyValue[1];
                            break;
                        case "voltage":
                            model.voltage = keyValue[1];
                            break;
                    }
                }
            }
        }

        //[HttpPost]
        //public IActionResult DrawRedDotOnMap(int x, int y)
        //{
        //    // Harita resminin yolu
        //    string mapPath = Path.Combine(Directory.GetCurrentDirectory(), "wwwroot\\images\\Harita.drawio.png");

        //    // Resmi yükle
        //    using (var bitmap = new Bitmap(mapPath))
        //    {
        //        // Bir Graphics nesnesi oluştur
        //        using (var graphics = Graphics.FromImage(bitmap))
        //        {
        //            // Kırmızı fırçayla X ve Y koordinatına bir daire çiz
        //            Brush redBrush = new SolidBrush(Color.Red);
        //            int dotSize = 10; // Noktanın büyüklüğü

        //            // X ve Y koordinatlarında merkezi olan bir daire çiz
        //            graphics.FillEllipse(redBrush, x - dotSize / 2, y - dotSize / 2, dotSize, dotSize);
        //        }

        //        // Yeni resmin yolunu ayarla
        //        string newMapPath = Path.Combine(Directory.GetCurrentDirectory(), "wwwroot/images/UpdatedMap.png");
        //        bitmap.Save(newMapPath);
        //    }

        //    // İşlenen yeni haritayı sayfaya geri döndür
        //    var model = new MessageModel
        //    {
        //        ImagePath = "/images/UpdatedMap.png"
        //    };

        //    return View("Index", model);
        //}




        public void ConvertPgmToPng(string pgmFilePath, string outputPath)
        {
            using (var reader = new StreamReader(pgmFilePath))
            {
                string magicNumber = reader.ReadLine();
                if (magicNumber != "P5")
                {
                    throw new InvalidDataException("Bu geçerli bir PGM dosyası değil.");
                }

                // Genişlik ve yükseklik
                string[] dimensions = reader.ReadLine().Split(' ');
                int width = int.Parse(dimensions[0]);
                int height = int.Parse(dimensions[1]);

                // Maksimum gri seviyesi
                int maxGrayValue = int.Parse(reader.ReadLine());

                // Resim verilerini okuyun
                byte[] imageData = new byte[width * height];
                reader.BaseStream.Read(imageData, 0, imageData.Length);

                // Bitmap'e dönüştürün
                using (var bitmap = new Bitmap(width, height, PixelFormat.Format8bppIndexed))
                {
                    var palette = bitmap.Palette;
                    for (int i = 0; i < 256; i++)
                    {
                        palette.Entries[i] = Color.FromArgb(i, i, i);
                    }
                    bitmap.Palette = palette;

                    var rect = new Rectangle(0, 0, width, height);
                    var bitmapData = bitmap.LockBits(rect, ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);

                    System.Runtime.InteropServices.Marshal.Copy(imageData, 0, bitmapData.Scan0, imageData.Length);
                    bitmap.UnlockBits(bitmapData);

                    // PNG olarak kaydedin
                    bitmap.Save(outputPath, ImageFormat.Png);
                }
            }

            //    try
            //    {
            //        using (var reader = new StreamReader(pgmFilePath))
            //        {
            //            string magicNumber = reader.ReadLine();
            //            if (magicNumber != "P5")
            //            {
            //                throw new InvalidDataException("Bu geçerli bir PGM dosyası değil.");
            //            }

            //            // Genişlik ve yükseklik
            //            string[] dimensions = reader.ReadLine().Split(' ');
            //            int width = int.Parse(dimensions[0]);
            //            int height = int.Parse(dimensions[1]);

            //            // Maksimum gri seviyesi
            //            int maxGrayValue = int.Parse(reader.ReadLine());
            //            if (maxGrayValue != 255)
            //            {
            //                throw new NotSupportedException("Bu program sadece 8-bit gri tonlu PGM dosyalarını destekler.");
            //            }

            //            // Resim verilerini okuyun
            //            byte[] imageData = new byte[width * height];
            //            reader.BaseStream.Read(imageData, 0, imageData.Length);

            //            // Bitmap'e dönüştürün
            //            using (var bitmap = new Bitmap(width, height, PixelFormat.Format8bppIndexed))
            //            {
            //                // Palette oluşturun
            //                ColorPalette palette = bitmap.Palette;
            //                for (int i = 0; i < 256; i++)
            //                {
            //                    palette.Entries[i] = Color.FromArgb(i, i, i);
            //                }
            //                bitmap.Palette = palette;

            //                // Bitmap veri işlemesi
            //                Rectangle rect = new Rectangle(0, 0, width, height);
            //                BitmapData bitmapData = bitmap.LockBits(rect, ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);

            //                // Her satırın başlangıç adresini ayarlamak için stride hesaplama
            //                int stride = bitmapData.Stride;
            //                IntPtr scan0 = bitmapData.Scan0;

            //                // imageData içeriğini bitmap'e kopyalama
            //                for (int y = 0; y < height; y++)
            //                {
            //                    IntPtr row = scan0 + (y * stride);
            //                    System.Runtime.InteropServices.Marshal.Copy(imageData, y * width, row, width);
            //                }

            //                bitmap.UnlockBits(bitmapData);

            //                // PNG olarak kaydedin
            //                bitmap.Save(outputPath, ImageFormat.Png);
            //            }
            //        }
            //    }
            //    catch (Exception ex)
            //    {
            //        // Hata yönetimi
            //        Console.WriteLine($"Bir hata oluştu: {ex.Message}");
            //        throw;
            //    }
            //}
        }

    }
}





