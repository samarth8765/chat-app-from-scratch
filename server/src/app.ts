import express, { Request, Response } from "express";

const app = express();
const PORT = 8080;

app.get("/", (req: Request, res: Response) => {
  console.log("Yo Baby");
  res.status(200).json("Yo whatsup");
});

app.listen(PORT, () => {
  console.log(`Listening at PORT ${PORT}`);
});
