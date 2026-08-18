import java.io.*;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.Base64;
import java.util.Date;

public class RequestHandler implements Runnable {
    private Socket clientSocket;
    private static final String UPLOAD_DIRECTORY = "uploaded";
    private static final int CHUNK_SIZE = 1024;
    private final BufferedWriter logWriter;

    public RequestHandler(Socket socket, BufferedWriter logWriter) {
        this.clientSocket = socket;
        this.logWriter = logWriter;
    }
    @Override
    public void run() {
        try {
            InputStream inputStream = clientSocket.getInputStream();
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(inputStream));
            String requestLine = in.readLine();
            if (requestLine == null || requestLine.isEmpty()) {
                sendErrorResponse(out, 400, "Bad Request");
                clientSocket.close();
                return;
            }
            logRequest(requestLine);
            String[] requestParts = requestLine.split(" ");
            String method = requestParts[0];
            String filePath = requestParts[1];
            switch (method) {
                case "GET":
                    handleGetRequest(out, filePath);
                    break;
                case "UPLOAD":
                    try{
                        handleUploadRequest(inputStream, out, filePath);
                    } catch (Exception e) {
                        System.out.println(e.getMessage());
                    }

                    break;
                default:
                    sendErrorResponse(out, 400, "Bad Request");
            }
            clientSocket.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private void handleGetRequest(PrintWriter out, String filePath) throws IOException {
        File file = new File("." + filePath);

        if (filePath.equals("/")) {
            filePath = "/";
            file = new File("." + filePath);
        } else if (filePath.startsWith("/uploaded/")) {
            file = new File(UPLOAD_DIRECTORY + filePath.substring("/uploaded".length()));
        }

        if (!file.exists()) {
            sendErrorResponse(out, 404, "Not Found");
            return;
        }

        if (file.isDirectory()) {
            serveDirectory(file, out);
        } else {
            serveFile(file, out);
        }
    }

    private void handleUploadRequest(InputStream inputStream, PrintWriter out, String filePath) throws IOException {
        File uploadDir = new File(UPLOAD_DIRECTORY);
        System.out.println(filePath);
        if (!uploadDir.exists()) {
            uploadDir.mkdirs();
        }

        if(!(filePath.endsWith("jpg") || filePath.endsWith("png") || filePath.endsWith("txt") || filePath.endsWith("mp4"))) {
            sendErrorResponse(out, 400, "Bad Request");
            System.out.println("client tried to upload a file which is not supported");
            return;
        }

        File uploadedFile = new File(uploadDir, filePath);
        try (BufferedOutputStream fileOut = new BufferedOutputStream(new FileOutputStream(uploadedFile))) {
            byte[] buffer = new byte[CHUNK_SIZE];
            int bytesRead;
            while ((bytesRead = inputStream.read(buffer)) != -1) {
                fileOut.write(buffer, 0, bytesRead);
            }
            fileOut.flush();
            Path file = Paths.get(uploadedFile.getAbsolutePath());
            String mimeType = Files.probeContentType(file);
            if (mimeType == null) {
                mimeType = "application/octet-stream";
            }
            out.println("HTTP/1.0 200 OK");
            out.println("Date: " + new Date());
            out.println("Content-Type: " + mimeType);
            out.println();
        }
    }

    private void serveDirectory(File dir, PrintWriter out) {
        out.println("HTTP/1.0 200 OK");
        out.println("Date: " + new Date());
        out.println("Content-Type: text/html");
        out.println();
        out.println("<html><body>");
        out.println("<h1>Directory listing for " + dir.getName() + "</h1>");
        out.println("<ul>");
        for (File file : dir.listFiles()) {
            if (file.isDirectory()) {
                out.println("<li><b><i><a href=\"" + file.getName() + "/\">" + file.getName() + "/</a></i></b></li>");
            } else {
                out.println("<li><a href=\"" + file.getName() + "\">" + file.getName() + "</a></li>");
            }
        }
        out.println("</ul>");
        out.println("</body></html>");
    }

    private void serveFile(File file, PrintWriter out) throws IOException {
        String contentType = getContentType(file);
        if (contentType.startsWith("text/") ) {
            serveTextFile(file, out, contentType);
        }
        else if(contentType.startsWith("image/")){
            serveImageFile(file, out, contentType);
        }else {
            serveFileDownload(out, file, contentType);
        }
    }
    private void serveImageFile(File file, PrintWriter out, String contentType) throws IOException {
        byte[] fileBytes = Files.readAllBytes(file.toPath());
        String base64Image = Base64.getEncoder().encodeToString(fileBytes);
        out.println("HTTP/1.0 200 OK");
        out.println("Date: " + new Date());
        out.println("Content-Type: text/html");
        out.println();
        out.println("<html>");
        out.println("<head><title>Image File Content</title></head>");
        out.println("<body>");
        out.println("<img src='data:" + contentType + ";base64," + base64Image + "'  width='100%' height='100%' alt='Image'/>");
        out.println("</body>");
        out.println("</html>");
    }

    private void serveTextFile(File file, PrintWriter out, String contentType) throws IOException {
        BufferedReader fileReader = new BufferedReader(new FileReader(file));
        StringBuilder fileContent = new StringBuilder();
        String line;
        while ((line = fileReader.readLine()) != null) {
            fileContent.append(line).append("<br>");
        }
        fileReader.close();
        out.println("HTTP/1.0 200 OK");
        out.println("Date: " + new Date());
        out.println("Content-Type: text/html");
        out.println();
        out.println("<html>");
        out.println("<head><title>Text File Content</title></head>");
        out.println("<body>");
        out.println("<pre>" + fileContent.toString() + "</pre>");
        out.println("</body>");
        out.println("</html>");
    }

    private void serveFileDownload(PrintWriter out, File file, String contentType) throws IOException {
        out.println("HTTP/1.0 200 OK");
        out.println("Date: " + new Date());
        out.println("Content-Type: " + contentType);
        out.println("Content-Length: " + file.length());
        out.println("Content-Disposition: attachment; filename=\"" + file.getName() + "\"");
        out.println();

        OutputStream os = clientSocket.getOutputStream();
        BufferedInputStream fileInput = new BufferedInputStream(new FileInputStream(file));
        byte[] buffer = new byte[1024];
        int bytesRead;
        while ((bytesRead = fileInput.read(buffer)) != -1) {
            os.write(buffer, 0, bytesRead);
        }
        os.flush();
        fileInput.close();
    }

    private void sendErrorResponse(PrintWriter out, int statusCode, String statusMessage) {
        String errorMessage = "<html><body><h1>" + statusCode + " " + statusMessage + "</h1></body></html>";
        out.println("HTTP/1.0 " + statusCode + " " + statusMessage);
        out.println("Date: " + new Date());
        out.println("Content-Type: text/html");
        out.println("Content-Length: " + errorMessage.length());
        out.println();
        out.println(errorMessage);
    }

    private String getContentType(File file) {
        String fileName = file.getName();
        if (fileName.endsWith(".html") || fileName.endsWith(".htm")) {
            return "text/html";
        } else if (fileName.endsWith(".jpg") || fileName.endsWith(".jpeg")) {
            return "image/jpeg";
        } else if (fileName.endsWith(".png")) {
            return "image/png";
        } else if (fileName.endsWith(".txt")) {
            return "text/plain";
        }
        return "application/octet-stream";
    }

    private void  logRequest(String requestLine) throws IOException {
        String clientAddress = clientSocket.getInetAddress().getHostAddress();
        String timestamp = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());

        synchronized (logWriter) {
            logWriter.write("[" + timestamp + "] " + clientAddress + " - " + requestLine);
            logWriter.newLine();
            logWriter.flush();
        }
    }

}