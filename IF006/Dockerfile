FROM gcc:13 AS build
WORKDIR /app
COPY Ordenacao.c .
RUN gcc -O2 -Wall -o ordenacao Ordenacao.c

FROM debian:bookworm-slim
WORKDIR /app
COPY --from=build /app/ordenacao .
COPY interface.html .
RUN useradd -r app && chown -R app /app
USER app
ENV PORT=8080
EXPOSE 8080
CMD ["./ordenacao"]